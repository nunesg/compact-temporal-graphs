import random


class TimeGenerator:
    def __init__(self):
        pass


class DummyGenerator(TimeGenerator):
    def __init__(self):
        super().__init__()

    def gen(self):
        return random.randrange(start=1, stop=5)
