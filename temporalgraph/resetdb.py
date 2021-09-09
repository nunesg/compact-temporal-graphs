from mysql import connector as con

db = con.connect(
    host="localhost",
    user="compact",
    password="compact",
    database="experiments"
)

mycursor = db.cursor()

try:
    mycursor.execute("DROP TABLE experiments_data;")
except:
    print(f"dropping failed")

mycursor.execute("SHOW TABLES;")
print(f'tables:\n{mycursor.fetchall()}')
