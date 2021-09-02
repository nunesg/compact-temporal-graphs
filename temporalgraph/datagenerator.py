
class DataGenerator:
    # graph is an adjacency list
    # V, E are the number of vertices and edges, respectively
    # datapath is the path in which to write the graph
    @staticmethod
    def gen(graph, V, E, T, datapath):
        str = f'{V} {E} {T}\n'
        for i in range(V):
            sz = len(graph[i])
            str += f'{sz}'
            for j in range(sz):
                v = graph[i][j][0]
                tbegin = graph[i][j][1]
                tend = graph[i][j][2]
                str += f' {v} {tbegin} {tend}'
            str += '\n'
        DataGenerator.write_file(datapath, str)

    @staticmethod
    def write_file(datapath, content):
        f = open(datapath, 'w')
        f.write(content)
        f.close()
