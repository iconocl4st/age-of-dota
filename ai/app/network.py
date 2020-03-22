
from .json_parser import LookAhead
from .json_parser import JsonParser
from .json_parser import JsonTextWriter
from .json_parser import JsonTokener
from .json_parser import JsonTokens
from .json_parser import SocketOutputWriter
from .json_parser import JsonSplitWriter
from .json_parser import FileOutputWriter

from .encoding_limits import Actions


class Message:
    Types = {}

    @staticmethod
    def get_name(msg_value):
        for key, value in Message.Types.items():
            if value == msg_value:
                return key
        raise Exception("Unknown message type: " + msg_value);


class ClientBuffer:
    def __init__(self, client_socket):
        self.client_socket = client_socket
        self.input_buffer = None
        self.input_buffer_index = 0

    def read(self):
        if self.input_buffer is None or self.input_buffer_index == len(self.input_buffer):
            self.input_buffer_index = 0
            self.input_buffer = self.client_socket.recv(1024)
            if not self.input_buffer:
                return None
            # print("Read: " + str(self.input_buffer))
        r = self.input_buffer[self.input_buffer_index]
        self.input_buffer_index += 1
        return r


class ClientConnection:
    def __init__(self, writer, parser):
        self.writer = writer
        self.parser = parser

    def maybe_parse_number(self, key, dicti):
        if self.parser.read_boolean(key="has-" + key):
            dicti[key] = self.parser.read_number(key=key)

    def parse_create_entity(self):
        entity = {
            'id': self.parser.read_number(key="entity-id"),
            'attributes': {}
        }
        with self.parser.read_begin_object(key="creation-params"):
            entity['type'] = self.parser.read_string(key="entity-spec")
            with self.parser.read_begin_object(key="location"):
                entity['attributes']['x'] = self.parser.read_number(key="x")
                entity['attributes']['y'] = self.parser.read_number(key="y")
            entity['attributes']['orientation'] = self.parser.read_number(key="orientation")
            entity['player'] = self.parser.read_number(key="player-number")

            self.maybe_parse_number("capacity", entity['attributes'])
            self.maybe_parse_number("accuracy", entity['attributes'])
            self.maybe_parse_number("collect-speed", entity['attributes'])
            self.maybe_parse_number("deposit-speed", entity['attributes'])
            self.maybe_parse_number("attack-speed", entity['attributes'])
            self.maybe_parse_number("line-of-sight", entity['attributes'])
            self.maybe_parse_number("max-health", entity['attributes'])
            self.maybe_parse_number("movement-speed", entity['attributes'])
            self.maybe_parse_number("rotation-speed", entity['attributes'])
        entity['attributes']['health'] = entity['attributes']['max-health']
        return entity

    def parse_delete_entity(self):
        entity_id = self.parser.read_number(key="entity-id")

    def parse_action_changed(self):
        changed = { 'action-args': {} }
        changed['entity-id'] = self.parser.read_number(key="entity-id")

        with self.parser.read_begin_object(key="action"):
            changed['action-type'] = self.parser.read_number(key="action-type")
            if changed['action-type'] == Actions.ATTACK:
                pass
            elif changed['action-type'] == Actions.IDLE:
                pass
            elif changed['action-type'] == Actions.STRAFE:
                changed['action-args']['is-left'] = self.parser.read_boolean(key='is-left')
            elif changed['action-type'] == Actions.COLLECT:
                changed['action-args']['resource'] = self.parser.read_number(key='resource')
            elif changed['action-type'] == Actions.DEPOSIT:
                changed['action-args']['resource'] = self.parser.read_number(key='resource')
            elif changed['action-type'] == Actions.MOVE:
                changed['action-args']['desired-orientation'] = self.parser.read_number(key='desired-orientation')
                changed['action-args']['path'] = []
                self.parser.read_begin_array(key='path')
                while not self.parser.is_end_array(JsonTokens.BEGIN_OBJECT):
                    point = {}
                    point['x'] = self.parser.read_number(key='x')
                    point['y'] = self.parser.read_number(key='y')
                    changed['action-args']['path'].append(point)
                    self.parser.read_end_object()
            else:
                raise Exception("Unknown action type: " + changed['action-type'])
        return changed

    def parse_movement_change(self):
        movement = { 'movement': {} }
        movement['entity'] = self.parser.read_number(key="entity-id")
        with self.parser.read_begin_object(key="movement"):
            with self.parser.read_begin_object(key="location-begin"):
                movement['movement']['x'] = self.parser.read_number(key="x")
                movement['movement']['y'] = self.parser.read_number(key="y")
            movement['movement']['direction-x'] = self.parser.read_number(key="direction-x")
            movement['movement']['direction-y'] = self.parser.read_number(key="direction-y")
            movement['movement']['move-speed'] = self.parser.read_number(key="movement-speed")
            movement['movement']['orientation'] = self.parser.read_number(key="orientation-begin")
            movement['movement']['rotation-direction'] = self.parser.read_number(key="rotation-direction")
            movement['movement']['rotation-speed'] = self.parser.read_number(key="rotation-speed")
            movement['movement']['start-time'] = self.parser.read_number(key="movement-begin")
        return movement

    def parse_game_time(self):
        new_game_time = {}
        new_game_time['time'] = self.parser.read_number("current-time")
        new_game_time['tick-time'] = self.parser.read_string("tick-time")
        return new_game_time

    def parse_projectile_launch(self):
        project_creation = {}
        with self.parser.read_begin_object(key="projectile"):
            project_creation['id'] = self.parser.read_number(key="id")
            project_creation['launcher'] = self.parser.read_number(key="launcher")
            project_creation['time'] = self.parser.read_number(key="launch-time")
            num_projectiles = self.parser.read_number(key="num-projectiles")
            project_creation['sunk'] = [False for i in range(num_projectiles)]
            project_creation['speed'] = self.parser.read_number(key="projectile-speed")
            project_creation['spread'] = self.parser.read_number(key="spread")
            project_creation['range'] = self.parser.read_number(key="range")
            project_creation['orientation'] = self.parser.read_number(key="orientation")
            with self.parser.read_begin_object(key="origin"):
                project_creation['x'] = self.parser.read_number(key="x")
                project_creation['y'] = self.parser.read_number(key="y")
        return project_creation

    def parse_projectile_update(self):
        update = {}
        update['id'] = self.parser.read_number(key="projectile-id")
        update['num'] = self.parser.read_number(key="projectile-num")
        return update

    def parse_projectile_destroy(self):
        return self.parser.read_number(key="projectile")

    def parse_close_game(self):
        with self.parser.read_begin_object("result"):
            return self.parser.read_number(key='winning-player-number')

    def parse_apply_damage(self):
        app = {}
        app['entity'] = self.parser.read_number(key="entity-id")
        with self.parser.read_begin_object(key="damage"):
            app['from'] = self.parser.read_number(key="from")
            with self.parser.read_begin_object(key="origin"):
                app['x'] = self.parser.read_number("x")
                app['y'] = self.parser.read_number("y")
            app['type'] = self.parser.read_number("damage-type")
            app['amount'] = self.parser.read_number("amount")
        with self.parser.read_begin_object(key="result"):
            app['health'] = self.parser.read_number("new-health")
            app['dies'] = self.parser.read_boolean("unit-dies")
        return app

    def parse_lobbies_list(self):
        lobbies = []
        self.parser.read_begin_array(key="lobbies")
        while not self.parser.is_end_array(alternative_token=JsonTokens.BEGIN_OBJECT):
            lobbies.append(self.parser.read_string("lobby-name"))
            self.parser.read_end_object()
        print("found lobbies", lobbies)
        return lobbies

    def parse_status_update(self):
        connected = self.parser.read_boolean("connected")
        ready = self.parser.read_boolean("ready")
        all_ready = self.parser.read_boolean("all-ready")
        spectating = self.parser.read_boolean("spectating")
        lobby_name = self.parser.read_string("lobby-name")
        return {
            "connected": connected,
            "ready": ready,
            "all_ready": all_ready,
            "spectating": spectating,
            "lobby_name": lobby_name,
        }

    def parse_launched(self):
        launch = {}

        self.parser.read_begin_object(key="launch-options")
        self.parser.read_begin_object(key="spec")
        launch['width'] = self.parser.read_number(key="width")
        launch['height'] = self.parser.read_number(key="height")
        self.parser.read_end_object()
        launch['player-number'] = self.parser.read_number(key="player-number")
        launch['spectating'] = self.parser.read_boolean(key="spectating")
        launch['num-players'] = self.parser.read_number(key="num-players")

        self.parser.read_end_object()
        return launch

    def send_noop(self):
        self.writer.write_begin_object()
        self.writer.write_number(key="message-type", value=Message.Types['NOOP'])
        self.writer.write_end_object()
        self.writer.flush()

    def send_close_connection(self):
        self.writer.write_begin_object()
        self.writer.write_number(key="message-type", value=Message.Types['CLOSE_CONNECTION'])
        self.writer.write_end_object()
        self.writer.flush()

    def set_ready(self, ready):
        self.writer.write_begin_object()
        self.writer.write_number(key="message-type", value=Message.Types['MARK_READY'])
        self.writer.write_boolean(key="ready", value=ready)
        self.writer.write_end_object()
        self.writer.flush()

    def create_lobby(self, lobby_name):
        self.writer.write_begin_object()
        self.writer.write_number(key="message-type", value=Message.Types['CREATE_LOBBY'])
        self.writer.write_string(key="lobby-name", value=lobby_name)
        self.writer.write_end_object()
        self.writer.flush()

    def join_lobby(self, lobby_name, player_number):
        self.writer.write_begin_object()
        self.writer.write_number(key="message-type", value=Message.Types['JOIN_LOBBY'])
        self.writer.write_string(key="lobby-name", value=lobby_name)
        self.writer.write_boolean(key="is-human", value=False)
        self.writer.write_number(key="player-number", value=player_number)
        self.writer.write_end_object()
        self.writer.flush()

    def list_lobbies(self):
        self.writer.write_begin_object()
        self.writer.write_number(key="message-type", value=Message.Types['LIST_LOBBIES'])
        self.writer.write_end_object()
        self.writer.flush()

    def send_actions_sent(self):
        self.writer.write_begin_object()
        self.writer.write_number(key="message-type", value=Message.Types['AI_ACTIONS_SENT'])
        self.writer.write_end_object()
        self.writer.flush()

    def send_action_request(self, entity_id, action):
        self.writer.write_begin_object()
        self.writer.write_number(key="message-type", value=Message.Types['REQUEST_ACTION'])
        self.writer.write_number(key="entity-id", value=entity_id)
        self.writer.write_begin_object(key="action")
        self.writer.write_number("action-type", action['action-type'])

        if action['action-type'] == Actions.IDLE:
            pass
        elif action['action-type'] == Actions.ATTACK:
            pass
        elif action['action-type'] == Actions.MOVE:
            self.writer.write_number(key="desired-orientation", value=action['action-args']['desired-orientation'])
            self.writer.write_begin_array(key='path')
            for point in action['action-args']['path']:
                self.writer.write_begin_object()
                self.writer.write_number(key="x", value=point['x'])
                self.writer.write_number(key="y", value=point['y'])
                self.writer.write_end_object()
            self.writer.write_end_array()
        elif action['action-type'] == Actions.COLLECT:
            self.writer.write_number(key="resource", value=action['action-args']['resource'])
        elif action['action-type'] == Actions.DEPOSIT:
            self.writer.write_number(key="resource", value=action['action-args']['resource'])
        elif action['action-type'] == Actions.STRAFE:
            self.writer.write_boolean(key="is-left", value=action['action-args']['is-left'])
        else:
            raise Exception("Invalid action")
        self.writer.write_end_object()
        self.writer.write_end_object()


def create_client_connection(socket, output_file):
    buffer = ClientBuffer(socket)
    lookAhead = LookAhead(buffer)
    tokener = JsonTokener(lookAhead)
    parser = JsonParser(tokener)
    writer = JsonSplitWriter(
        JsonTextWriter(SocketOutputWriter(socket)),
        JsonTextWriter(FileOutputWriter(output_file))
    )
    return ClientConnection(writer, parser)
