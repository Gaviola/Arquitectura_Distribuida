"""******************************************
La función obtener_IP() obtiene la dirección IP local
Para ello, se conecta a un servidor http conocido (IP de uncuyo.edu.ar), puerto 80.
Luego obtiene la IP local. 
No recibe argumentos. Retorna como respuesta una cadena de caracteres con la IP local. 
*******************************************"""
import socket
def obtener_IP():
  mi_socket=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  mi_socket.connect(("179.0.132.58",80))
  direccion_IP=mi_socket.getsockname()[0]
  mi_socket.close()
  return direccion_IP
  

  

