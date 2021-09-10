import random

# generate a 0-indexed temporal graph with distinct positive timestamps


class GraphGenerator:

    # return a list of 2*E tuples representing the events in the following format:
    # (t, u, v)
    # where u, v are the vertices (0-indexed)
    # t is the positive timestamp of the event on the interval [0, T]
    # the event list is sorted by the timestamps
    @staticmethod
    def gen_events(V, E, T):
        neighbours = [GraphGenerator.get_neighbours(V) for i in range(E)]
        events = GraphGenerator.add_timestamps(neighbours, T)
        return events

    # return an adjacency list for each vertex of the graph
    # adj[u]: [(v1, tbegin1, tend1), (v2, tbegin2, tend2), ...]
    # for each vertex u and its neighbours
    @staticmethod
    def gen_adjacencies(V, E, T):
        events = GraphGenerator.gen_events(V, E, T)
        adj = [[] for i in range(V)]
        m = dict()
        for event in events:
            t, u, v = event

            if (u, v) in m:
                adj[u].append((v, m.get((u, v)), t))
                m.pop((u, v))
            else:
                m[(u, v)] = t
        return adj

    # return a list of pairs [u, v] where 0 <= u,v < V
    @staticmethod
    def get_neighbours(V):
        return [random.randrange(start=0, stop=V) for i in range(2)]

    # create temporal events for the edges, sorted by timestamp
    # if the edges array has size E, then it will return 2*E events
    # in the form of (t, u, v), where 0 <= t <= T is the timestamp
    @staticmethod
    def add_timestamps(edges, T):
        events = []
        for pair in edges:
            u, v = pair
            events.append((random.randint(0, T), u, v))
            events.append((random.randint(0, T), u, v))
        events.sort(key=lambda e: e[0])
        return events
