import unittest

import pygambit


class TestGambitStrategySupportProfile(unittest.TestCase):
    def setUp(self):
        self.game = pygambit.Game.read_game("test_games/mixed_strategy.nfg")
        self.support_profile = self.game.support_profile()
        self.restriction = self.support_profile.restrict()

    def tearDown(self):
        del self.game
        del self.support_profile
        del self.restriction

    def test_num_strategies(self):
        """Ensure the support profile of the full game still has all strategies"""
        assert len(self.support_profile) == len(self.game.strategies)
        assert len(self.support_profile) == len(self.restriction.strategies)

    def test_remove(self):
        """Test removing strategies from a support profile"""
        strategy = self.support_profile[0]
        new_profile = self.support_profile.remove(strategy)
        assert len(self.support_profile) == len(new_profile) + 1
        assert strategy not in new_profile

    def test_difference(self):
        """Test the subtraction of two support profiles"""
        strat_list = [self.support_profile[0], self.support_profile[4]]
        dif_profile = pygambit.StrategySupportProfile(
            strat_list, self.game)
        new_profile = self.support_profile - dif_profile
        assert len(new_profile) == 3
        for strategy in strat_list:
            assert strategy not in new_profile

    def test_difference_error(self):
        """Ensure an error is raised when the difference isn't a
        valid support profile
        """
        def foo():
            strat_list = [self.support_profile[0], self.support_profile[4]]
            dif_profile = pygambit.StrategySupportProfile(
                strat_list, self.game
            )
            dif_profile - self.support_profile
        self.assertRaises(ValueError, foo)

    def test_intersection(self):
        """Test the intersection between two support profiles"""
        strat_list = [self.support_profile[0], self.support_profile[2],
                      self.support_profile[4]]
        fir_profile = pygambit.StrategySupportProfile(
            strat_list, self.game)
        sec_profile = self.support_profile.remove(self.support_profile[2])
        new_profile = fir_profile & sec_profile
        assert len(new_profile) == 2
        assert new_profile <= sec_profile
        assert new_profile <= fir_profile

    def test_intersection_error(self):
        """Ensure an error is raised when the intersection isn't a
        valid support profile.
        """
        def foo():
            strat_list = [self.support_profile[0], self.support_profile[2],
                          self.support_profile[4]]
            fir_profile = pygambit.StrategySupportProfile(
                strat_list, self.game
            )
            sec_profile = self.support_profile.remove(self.support_profile[4])
            fir_profile & sec_profile
        self.assertRaises(ValueError, foo)

    def test_union(self):
        """Test the union between two support profiles"""
        strat_list = [self.support_profile[0], self.support_profile[2],
                      self.support_profile[4]]
        fir_profile = pygambit.StrategySupportProfile(
            strat_list, self.game)
        sec_profile = self.support_profile.remove(self.support_profile[4])
        new_profile = fir_profile | sec_profile
        assert new_profile == self.support_profile

    def test_undominated(self):
        """Test removing undominated strategies from the support profile"""
        new_profile = self.support_profile
        loop_profile = pygambit.supports.undominated_strategies_solve(new_profile)
        while loop_profile != new_profile:
            new_profile = loop_profile
            loop_profile = pygambit.supports.undominated_strategies_solve(new_profile)
        assert len(loop_profile) == 2
        assert loop_profile == pygambit.StrategySupportProfile(
            [self.support_profile[0], self.support_profile[3]], self.game)

    def test_remove_error(self):
        """Test removing the last strategy of a player"""
        def foo():
            profile = self.support_profile.remove(self.support_profile[3])
            profile.remove(profile[3])
        self.assertRaises(pygambit.UndefinedOperationError, foo)
