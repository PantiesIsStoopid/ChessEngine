import tkinter as tk
import time
import os
import threading

# Map pieces to their respective image files
piece_images = {
    'P': 'images/wP.png',  # White Pawn
    'N': 'images/wN.png',  # White Knight
    'B': 'images/wB.png',  # White Bishop
    'R': 'images/wR.png',  # White Rook
    'Q': 'images/wQ.png',  # White Queen
    'K': 'images/wK.png',  # White King
    'p': 'images/bP.png',  # Black Pawn
    'n': 'images/bN.png',  # Black Knight
    'b': 'images/bB.png',  # Black Bishop
    'r': 'images/bR.png',  # Black Rook
    'q': 'images/bQ.png',  # Black Queen
    'k': 'images/bK.png',  # Black King
    '.': ''  # Empty space (no image)
}

def parse_board(file_path):
    with open(file_path, "r") as f:
        lines = f.readlines()

    # Extract the board from the lines (skip the first and last lines)
    board = []
    for i in range(8):  # Only consider the 8 lines corresponding to the board
        line = lines[i].strip()
        # Skip empty lines
        if not line:
            print(f"Warning: Empty line found at line {i+1}. Skipping...")
            continue
        
        # Split the line into pieces, skipping the rank number
        row = line.split()[1:]  # Skip the rank number part and take the rest
        if len(row) != 8:
            print(f"Warning: Invalid row format at line {i+1}. Expected 8 pieces, got {len(row)}.")
            continue
        
        board.append(row)

    return board

def render_board_gui(board, board_frame):
    # Clear the current labels on the board
    for widget in board_frame.winfo_children():
        widget.destroy()

    # Create a label for each square
    for rank in range(8):  # Display rank from 1 to 8 (with white at the bottom)
        for file in range(8):
            piece = board[7 - rank][file]  # Reverse the rank order to ensure white at the bottom
            img_file = piece_images.get(piece, '')  # Get the image for the piece
            if img_file:
                img = tk.PhotoImage(file=img_file)  # Load the image
                label = tk.Label(board_frame, image=img, width=60, height=60)
                label.image = img  # Keep a reference to the image to prevent garbage collection
            else:
                label = tk.Label(board_frame, text='', width=4, height=2, font=("Arial", 18))

            label.grid(row=rank, column=file, padx=1, pady=1)

def render_chess_board_from_file(file_path, board_frame):
    board = parse_board(file_path)
    render_board_gui(board, board_frame)

def monitor_file_for_changes(file_path, board_frame, update_callback):
    # Track the initial modification time of the file
    last_modified_time = os.path.getmtime(file_path)

    while True:
        # Check if the file has been modified
        current_modified_time = os.path.getmtime(file_path)
        
        if current_modified_time != last_modified_time:
            print("File changed, updating board...")
            update_callback(file_path, board_frame)  # Trigger the callback to update the board
            last_modified_time = current_modified_time
        
        time.sleep(1)  # Check every second

def create_gui(file_path):
    # Create the main window using Tkinter
    root = tk.Tk()
    root.title("Chess Board")

    # Create a frame to hold the board
    board_frame = tk.Frame(root)
    board_frame.grid(row=0, column=0)

    # Render the initial board
    render_chess_board_from_file(file_path, board_frame)

    # Start a new thread to monitor the file for changes
    monitor_thread = threading.Thread(target=monitor_file_for_changes, args=(file_path, board_frame, render_chess_board_from_file))
    monitor_thread.daemon = True  # Ensure the thread will exit when the program ends
    monitor_thread.start()

    # Run the Tkinter event loop
    root.mainloop()

# File path to your board output text file
file_path = "board_output.txt"

# Create the GUI and monitor the file for changes
create_gui(file_path)
