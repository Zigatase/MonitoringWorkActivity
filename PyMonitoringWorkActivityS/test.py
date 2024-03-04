# import socket
# import threading
# import tkinter as tk
# from os import remove
# from tkinter import ttk, messagebox
#
# from PIL import Image, ImageTk
#
#
# class Server:
#     def __init__(self, host, port):
#         # --- Server ---
#         self.host = host
#         self.port = port
#         self.clients = []
#         self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#         self.server_socket.bind((self.host, self.port))
#         self.server_socket.listen(5)
#
#         # --- GUI ---
#         self.window = tk.Tk()
#         self.window.title("Server")
#         self.window.protocol("WM_DELETE_WINDOW", self.on_closing)
#
#         self.domain_client = list()
#         self.machine_client = list()
#         self.ip_client = list()
#         self.user_client = list()
#         self.client_status = list()
#         self.client_socket = list()
#
#         # TreeView
#         self.client_conn_table = ttk.Treeview(self.window, columns=('Domain', 'Machine', 'IP', 'User', 'Status'),
#                                               show='headings')
#         self.client_conn_table.heading('Domain', text='Domain')
#         self.client_conn_table.heading('Machine', text='Machine')
#         self.client_conn_table.heading('IP', text='IP')
#         self.client_conn_table.heading('User', text='User')
#         self.client_conn_table.heading('Status', text='Status')
#
#         # Event
#         self.client_conn_table.bind('<<TreeviewSelect>>', self.item_select)
#
#         self.client_conn_table.pack()
#
#     def accept_clients(self):
#         while True:
#             try:
#                 client_socket, client_address = self.server_socket.accept()
#                 self.clients.append((client_socket, client_address))
#                 self.client_socket.append(client_socket)
#                 threading.Thread(target=self.handle_client, args=(client_socket,)).start()
#             except ConnectionResetError:
#                 print("[accept_clients] Client forcibly disconnected")
#
#     def handle_client(self, client_socket):
#         while True:
#             try:
#                 data = client_socket.recv(1024000)
#                 if not data:
#                     print("Client disconnected:", client_socket.getpeername())
#                     self.client_socket[self.client_socket.index(client_socket)] = "NONE"
#                     client_socket.close()
#                     self.clients.remove((client_socket,))
#                     break
#
#                 # --- ---
#                 try:
#                     if data.decode()[0] == "C":
#                         message: str = data.decode()
#
#                         if self.machine_client.count(message.split()[2]) == 0:
#                             self.domain_client.append(message.split()[1])
#                             self.machine_client.append(message.split()[2])
#                             self.ip_client.append(message.split()[3])
#                             self.user_client.append(message.split()[4])
#                             self.client_status.append("Online")
#
#                             index = self.machine_client.index(message.split()[2])
#                             self.client_socket[index] = client_socket
#
#                             self.update_clients_list()
#
#                         elif self.machine_client.count(message.split()[2]) == 1:
#                             index = self.machine_client.index(message.split()[2])
#
#                             self.client_status[index] = "Online"
#                             self.client_socket[index] = client_socket
#
#                             self.update_clients_list()
#                 # --- ScreenShot ---
#                 except UnicodeDecodeError:
#                     self.show_screenshot(data)
#
#             except ConnectionResetError:
#                 print(f"[handle_client] Client forcibly disconnected {message}")
#
#                 index = self.machine_client.index(message.split()[2])
#
#                 self.client_status[index] = "Offline"
#                 self.client_socket[index] = "NONE"
#
#                 self.update_clients_list()
#                 break
#
#     def update_clients_list(self) -> None:
#         self.client_conn_table.destroy()
#         self.treeview()
#
#         for i in range(len(self.domain_client)):
#             data = (
#                 self.domain_client[i],
#                 self.machine_client[i],
#                 self.ip_client[i],
#                 self.user_client[i],
#                 self.client_status[i]
#             )
#
#             self.client_conn_table.insert(parent='', index=i, values=data)
#
#     def item_select(self, _):
#         try:
#             print(self.client_conn_table.selection())
#             index_str = self.client_conn_table.selection()[0]
#             index = int(index_str[len("I"):]) - 1
#
#             if self.client_socket[index] != "NONE":
#                 self.client_socket[index].send("-ScreenShot".encode())
#             else:
#                 messagebox.showerror("Client disconnected", "Клиент отключился, создание скриншота невозможно")
#
#         except ValueError:
#             print("[item_select -> ValueError]: Invalid selection")
#
#     def show_screenshot(self, image_bytes) -> None:
#         with open('image.jpg', 'wb') as f:
#             f.write(image_bytes)
#
#         self.img = Image.open("image.jpg")
#         self.image = ImageTk.PhotoImage(self.img)
#
#         top = tk.Toplevel(self.window)
#         top.title = "Screenshot client"
#         canvas = tk.Canvas(top, width=self.image.width(), height=self.image.height())
#         canvas.create_image(0, 0, anchor="nw", image=self.image)
#
#         canvas.pack()
#
#         remove("image.jpg")
#
#     def treeview(self):
#         # TreeView
#         self.client_conn_table = ttk.Treeview(self.window, columns=('Domain', 'Machine', 'IP', 'User', 'Status'),
#                                               show='headings')
#         self.client_conn_table.heading('Domain', text='Domain')
#         self.client_conn_table.heading('Machine', text='Machine')
#         self.client_conn_table.heading('IP', text='IP')
#         self.client_conn_table.heading('User', text='User')
#         self.client_conn_table.heading('Status', text='Status')
#
#         # Event
#         self.client_conn_table.bind('<<TreeviewSelect>>', self.item_select)
#         self.client_conn_table.pack()
#
#     def on_closing(self):
#         self.server_socket.close()
#         self.window.destroy()
#
#     def start_server(self):
#         threading.Thread(target=self.accept_clients, daemon=True).start()
#         self.window.mainloop()
#
#
# if __name__ == "__main__":
#     server = Server('localhost', 55555)
#     server.start_server()
