from flask import Flask, request, jsonify
import sqlite3

app= Flask(__name__)

ARCHIVO_BD = "sensores.db"

@app.route('/')
def hello_world():
   return '<p>Hello, World!</p>'

def make_dicts(cursor, row):
   return dict((cursor.description[idx][0], value)
               for idx, value in enumerate(row))

@app.route("/api/lecturas")
def lecturas():
   db = sqlite3.connect(ARCHIVO_BD)
   db.row_factory = make_dicts
   try:
        cursor = db.execute("""SELECT  id, nombre, valor, 
		                    datetime(fecha_hora, "-3 hours")
                            AS fecha_hora
	                        FROM lecturas;""")
        resultado = cursor.fetchall()
#    print("GET a lecturas")
#    print(resultado)
#   except:
#        return "No se pudo conectar a la base de datos",500
   finally:
        db.close
   return jsonify(resultado)

@app.route('/api/sensor', methods=['POST'])
def recibir_valores():
   datos= request.json
   nombre= datos['nombre']
   valor= datos['valor']

   db = sqlite3.connect(ARCHIVO_BD)
   db.row_factory = make_dicts
   try:
        db.execute("""INSERT INTO lecturas (nombre, valor) 
                            VALUES (?,?);""", (nombre, valor))
        db.commit()
   finally:
        db.close()

   return 'OK'