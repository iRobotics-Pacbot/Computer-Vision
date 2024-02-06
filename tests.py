import unittest

from cv_arena import wall_correction, trace_missing_path


class TestWallCorrection(unittest.TestCase):
    def test_internal_walls(self):
        '''
        Test the function deals with walls at the center of the arena correctly
        '''
        self.assertEqual(wall_correction((17, 10)), (17, 11))

    def test_outer_walls(self):
        '''
        Test the function deals with walls at the edge of the arena correctly
        '''
        self.assertEqual(wall_correction((5, 15)), (6, 15))

    def test_corner_walls(self):
        '''
        Test the function deals with walls at the corner of the arena correctly, both at the inside of a corner and outside of a corner
        '''
        self.assertEqual(wall_correction((0, 0)), (1, 1), "Top left corner")
        self.assertEqual(wall_correction((27, 0)), (26, 1), "Top right corner")
        self.assertEqual(wall_correction((0, 30)),
                         (1, 29), "Bottom left corner")
        self.assertEqual(wall_correction((27, 30)),
                         (26, 29), "Bottom right corner")
        self.assertIn(wall_correction((5, 2)), [
                      (5, 1), (6, 1), (6, 2)], "ambiguous inner corner with 3 possible final locations")

    def test_other_edge_cases(self):
        '''
        Other edge cases difficult to categorize
        '''
        pass

    def test_illegal_input(self):
        '''
        test illegal inputs are correctly dealt with
        '''
        self.assertRaises(ValueError, wall_correction, (-1, -1))


class TestTracePath(unittest.TestCase):
    def test_straight_path(self):
        '''
        test nodes that form a straight path are correctly dealt with
        '''
        self.assertEqual(trace_missing_path(
            (1, 1), (3, 1)), [(1, 1), (2, 1), (3, 1)])

    def test_corner_path(self):
        '''
        test nodes that forms a path with turns are correctly dealt with
        '''
        self.assertEqual(trace_missing_path(
            (1, 2), (2, 1)), [(1, 2), (1, 1), (2, 1)])

    def test_long_path(self):
        '''
        test a longer sequence of path
        '''
        self.assertEqual(trace_missing_path((16, 1), (23, 5)), [(16, 1), (17, 1), (18, 1), (
            19, 1), (20, 1), (21, 1), (21, 2), (21, 3), (21, 4), (21, 5), (22, 5), (23, 5)])


if __name__ == '__main__':
    unittest.main()
