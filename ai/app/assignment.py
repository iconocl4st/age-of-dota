
import numpy as np
from .encoding_limits import NumpyLimits

def set_player_to_zero(player_to_idx, player_number):
    cAssign = -1
    for idx, assignment in enumerate(player_to_idx):
        if assignment == 0:
            cAssign = idx
    if cAssign == player_number:
        return

    player_to_idx[cAssign] = player_to_idx[player_number]
    player_to_idx[player_number] = 0


class Assignment:
    def __init__(self):
        self._player_mapping = np.random.choice(
            NumpyLimits.MAX_NUM_PLAYERS,
            NumpyLimits.MAX_NUM_PLAYERS,
            replace=False
        )
        self.cAssign = -1
        for idx, assignment in enumerate(self._player_mapping):
            if assignment == 0:
                self.cAssign = idx

        self.entity_mapping = np.array([
            np.random.choice(
                NumpyLimits.MAX_NUM_ENTITIES_PER_PLAYER,
                NumpyLimits.MAX_NUM_ENTITIES_PER_PLAYER,
                replace=False
            ) for i in range(NumpyLimits.MAX_NUM_PLAYERS)])

        self.projectile_mapping = np.random.choice(
            NumpyLimits.MAX_NUM_PROJECTILES,
            NumpyLimits.MAX_NUM_PROJECTILES,
            replace=False
        )

    def get_player_index(self, player, player_number):
        if player_number == player:
            return 0
        if player == self.cAssign:
            return self._player_mapping[0]
        return self._player_mapping[player]
