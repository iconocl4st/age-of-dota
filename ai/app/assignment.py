
import numpy as np
from .encoding_limits import NumpyLimits


class ReproducibleAssignment:
    def __init__(self, max_num, required_to_be_zero=None):
        self.max = max_num
        self.assignment_cache = {}
        self.already_assigned = set()

        if required_to_be_zero is not None:
            self.assignment_cache[required_to_be_zero] = 0
            self.already_assigned.add(0)

    def get(self, ident):
        if ident in self.assignment_cache:
            return self.assignment_cache[ident]

        if len(self.already_assigned) >= self.max:
            raise Exception()

        assignment = np.random.randint(0, self.max)
        while assignment in self.already_assigned:
            assignment += 1
            if assignment >= self.max:
                assignment = 0

        self.assignment_cache[ident] = assignment
        self.already_assigned.add(assignment)

        return assignment


class Assignment:
    def __init__(self, player_number):
        self.player_assignments = ReproducibleAssignment(NumpyLimits.MAX_NUM_PLAYERS, player_number)
        self.entity_assignments = [ReproducibleAssignment(NumpyLimits.MAX_NUM_ENTITIES_PER_PLAYER) for _ in range(NumpyLimits.MAX_NUM_PLAYERS)]
        self.projectile_assignments = ReproducibleAssignment(NumpyLimits.MAX_NUM_PROJECTILES)

    def get_player_index(self, player):
        return self.player_assignments.get(player)

    def get_entity_index(self, player_index, entity_id):
        return self.entity_assignments[player_index].get(entity_id)

    def get_projectile_index(self, projectile_id):
        return self.projectile_assignments.get(projectile_id)

