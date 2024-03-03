import tkinter as tk
from tkinter import ttk
import socket
import threading


class Server:
    def __init__(self, host, port):
        # --- Server ---
        self.host = host
        self.port = port
        self.clients = []
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen(5)

        # --- GUI ---
        self.window = tk.Tk()
        self.window.title("Server")
        self.window.protocol("WM_DELETE_WINDOW", self.on_closing)

        self.domain_client = list()
        self.machine_client = list()
        self.ip_client = list()
        self.user_client = list()
        self.client_status = list()
        self.client_socket = list()

        # TreeView
        self.client_conn_table = ttk.Treeview(self.window, columns=('Domain', 'Machine', 'IP', 'User', 'Status'), show='headings')
        self.client_conn_table.heading('Domain', text='Domain')
        self.client_conn_table.heading('Machine', text='Machine')
        self.client_conn_table.heading('IP', text='IP')
        self.client_conn_table.heading('User', text='User')
        self.client_conn_table.heading('Status', text='Status')

        # Event
        self.client_conn_table.bind('<<TreeviewSelect>>', self.item_select)

        self.client_conn_table.pack()

    def accept_clients(self):
        while True:
            try:
                client_socket, client_address = self.server_socket.accept()
                self.clients.append((client_socket, client_address))
                self.client_socket.append(client_socket)
                threading.Thread(target=self.handle_client, args=(client_socket,)).start()
            except ConnectionResetError:
                print("[accept_clients] Client forcibly disconnected")

    def handle_client(self, client_socket):
        while True:
            try:
                data = client_socket.recv(1024000)
                if not data:
                    print("Client disconnected:", client_socket.getpeername())
                    client_socket.close()
                    self.clients.remove((client_socket,))
                    break

                # --- ---
                try:
                    if len(data.decode().split()) > 1:
                        message: str = data.decode()

                        if self.machine_client.count(message.split()[1]) == 0:
                            self.domain_client.append(message.split()[0])
                            self.machine_client.append(message.split()[1])
                            self.ip_client.append(message.split()[2])
                            self.user_client.append(message.split()[3])
                            self.client_status.append("Online")

                            self.update_clients_list()

                        elif self.machine_client.count(message.split()[1]) == 1:
                            index = self.machine_client.index(message.split()[1])
                            self.client_status[index] = "Online"
                            self.update_clients_list()
                except UnicodeDecodeError:
                    # Получить данные изображения
                    img = data
                    # Сохранить изображение
                    with open('image.jpg', 'wb') as f:
                        f.write(img)

            except ConnectionResetError:
                print(f"[handle_client] Client forcibly disconnected {message}")
                index = self.machine_client.index(message.split()[1])
                self.client_status[index] = "Offline"
                self.update_clients_list()
                break

    def update_clients_list(self) -> None:
        self.client_conn_table.delete(*self.client_conn_table.get_children())
        for i in range(len(self.domain_client)):
            data = (
                self.domain_client[i],
                self.machine_client[i],
                self.ip_client[i],
                self.user_client[i],
                self.client_status[i]
            )

            self.client_conn_table.insert(parent='', index=0, values=data)

    def item_select(self, _):
        for i in self.client_conn_table.selection():
            print(self.client_conn_table.item(i))

        # Request screen
        self.client_socket[0].send("-ScreenShot".encode())

    def on_closing(self):
        self.server_socket.close()
        self.window.destroy()

    def start_server(self):
        threading.Thread(target=self.accept_clients, daemon=True).start()
        self.window.mainloop()


if __name__ == "__main__":
    server = Server('localhost', 55555)
    server.start_server()
