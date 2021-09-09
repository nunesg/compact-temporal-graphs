from mysql import connector as con
import re

CREATE_DATABASE_QUERY = "CREATE DATABASE IF NOT EXISTS experiments"
CREATE_TABLE_QUERY = '''
  CREATE TABLE IF NOT EXISTS experiments_data(
    graph_type          VARCHAR(50),
    V                   INT,
    E                   INT,
    T                   INT,
    build_time_ms       FLOAT,
    has_edge_time_ms    FLOAT,
    neighbours_time_ms  FLOAT,
    aggregate_time_ms   FLOAT,
    graph_rss_kb        INT,
    max_rss_kb          INT,
    has_edge_epochs     INT,
    neighbours_epochs   INT,
    aggregate_epochs    INT,
    PRIMARY KEY (graph_type, V, E, T)
)
'''

GET_TABLE_COLUMNS_QUERY = '''
SELECT COLUMN_NAME
FROM INFORMATION_SCHEMA.COLUMNS
WHERE TABLE_NAME = N'experiments_data'
ORDER BY ORDINAL_POSITION
'''

SELECT_TABLE_QUERY = '''
SELECT * FROM experiments_data
'''


def get_insert_query(values):
    return f'''
    INSERT IGNORE INTO experiments_data
    VALUES
    (
      '{values["graph_type"]}',
      {values["V"]},
      {values["E"]},
      {values["T"]},
      {values["build_time_ms"]},
      {values["has_edge_time_ms"]},
      {values["neighbours_time_ms"]},
      {values["aggregate_time_ms"]},
      {values["graph_rss_kb"]},
      {values["max_rss_kb"]},
      {values["has_edge_epochs"]},
      {values["neighbours_epochs"]},
      {values["aggregate_epochs"]}
    )
  '''


class DatabaseManager:
    def __init__(self):
        self.db = None
        self.cursor = None
        self.setup()

    def setup(self):
        tmpcon = con.connect(
            host="localhost",
            user="compact",
            password="compact"
        )
        tmpcon.cursor().execute(CREATE_DATABASE_QUERY)
        tmpcon.close()

        self.db = con.connect(
            host="localhost",
            user="compact",
            password="compact",
            database="experiments"
        )

        self.cursor = self.db.cursor()

        self.cursor.execute("SHOW DATABASES")
        print(f'databases:\n{self.cursor.fetchall()}')
        self.cursor.execute(CREATE_TABLE_QUERY)
        self.cursor.execute("SHOW TABLES")
        print(f'tables:\n{self.cursor.fetchall()}')

    def insert(self, data):
        self.cursor.execute(get_insert_query(data))
        self.db.commit()
        self.print_table()

    def print_table(self):
        print(f"experiments_data:\n")
        self.cursor.execute(GET_TABLE_COLUMNS_QUERY)
        headers = self.rename_headers(self.cursor.fetchall())
        self.cursor.execute(SELECT_TABLE_QUERY)
        table = self.cursor.fetchall()
        print(headers)
        for r in table:
            print(r)

    def rename_headers(self, headers):
        headers = [re.sub(r'[^\w]', '', f"{h}") for h in headers]
        headers = [re.sub(r'graph', 'g', f"{h}") for h in headers]
        headers = [re.sub(r'has_edge', 'hedg', f"{h}") for h in headers]
        headers = [re.sub(r'neighbour', 'neigh', f"{h}") for h in headers]
        headers = [re.sub(r'aggregate', 'agg', f"{h}") for h in headers]
        headers = [re.sub(r'time', 't', f"{h}") for h in headers]
        headers = [re.sub(r'epochs', 'ep', f"{h}") for h in headers]
        return headers

    def close(self):
        self.db.commit()
        self.cursor.close()
        self.db.close()
