from mysql import connector as con

tmpcon = con.connect(
    host="localhost",
    user="compact",
    password="compact"
)
tmpcon.cursor().execute("CREATE DATABASE IF NOT EXISTS experiments;")
tmpcon.close()

db = con.connect(
    host="localhost",
    user="compact",
    password="compact",
    database="experiments"
)

mycursor = db.cursor()

mycursor.execute("SHOW DATABASES;")
print(f'databases:\n{mycursor.fetchall()}')
mycursor.execute(
    '''
CREATE TABLE experiments_data(
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
  aggregate_epochs    INT
)
''')
mycursor.execute("SHOW TABLES;")
print(f'tables:\n{mycursor.fetchall()}')
