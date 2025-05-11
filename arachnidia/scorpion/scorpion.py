import tkinter as tk
from tkinter import filedialog, messagebox
from PIL import Image, ExifTags
import argparse
import os
import sys

# Globals
exif_map = {}
current_image = None
current_path = ""
image_paths = []
current_index = 0

def open_image(index=None):
    global current_image, current_path, current_index

    if index is not None:
        if 0 <= index < len(image_paths):
            current_index = index
            path = image_paths[current_index]
        else:
            return
    else:
        path = filedialog.askopenfilename(filetypes=[("JPEG Images", "*.jpg;*.jpeg")])
        if path:
            image_paths.clear()
            image_paths.append(path)
            current_index = 0
        else:
            return

    if not os.path.exists(path):
        messagebox.showerror("Erreur", "Fichier non trouvé.")
        return

    image = Image.open(path)
    current_image = image
    current_path = path

    exif_data = image.getexif()
    listbox.delete(0, tk.END)
    exif_map.clear()

    for tag_id, value in exif_data.items():
        tag_name = ExifTags.TAGS.get(tag_id, f"Tag {tag_id}")
        exif_map[tag_name] = (tag_id, value)
        listbox.insert(tk.END, f"{tag_name}: {value}")

    root.title(f"Éditeur EXIF – {os.path.basename(current_path)}")

def edit_metadata():
    selection = listbox.curselection()
    if not selection:
        return

    index = selection[0]
    selected = listbox.get(index)
    key = selected.split(":")[0]
    tag_id, old_value = exif_map[key]

    new_value = entry.get()
    if new_value:
        current_image.getexif()[tag_id] = new_value
        listbox.delete(index)
        listbox.insert(index, f"{key}: {new_value}")
        messagebox.showinfo("Modifié", f"'{key}' mis à jour.")
    else:
        messagebox.showwarning("Erreur", "Valeur vide.")

def delete_metadata():
    selection = listbox.curselection()
    if not selection:
        return

    index = selection[0]
    selected = listbox.get(index)
    key = selected.split(":")[0]
    tag_id, _ = exif_map[key]

    del current_image.getexif()[tag_id]
    listbox.delete(index)
    messagebox.showinfo("Supprimé", f"'{key}' supprimé.")

def save_image():
    save_path = filedialog.asksaveasfilename(defaultextension=".jpg")
    if not save_path:
        return
    current_image.save(save_path, exif=current_image.getexif())
    messagebox.showinfo("Sauvegarde", "Image sauvegardée avec succès.")

def next_image():
    if current_index + 1 < len(image_paths):
        open_image(current_index + 1)

def prev_image():
    if current_index - 1 >= 0:
        open_image(current_index - 1)

# GUI
def launch_gui(paths=None):
    global listbox, entry, root, image_paths

    root = tk.Tk()
    root.title("Éditeur EXIF")

    frame = tk.Frame(root)
    frame.pack(padx=10, pady=10)

    listbox = tk.Listbox(frame, width=60, height=15)
    listbox.grid(row=0, column=0, columnspan=4)

    entry = tk.Entry(frame, width=40)
    entry.grid(row=1, column=0, columnspan=3, pady=5)

    tk.Button(frame, text="Modifier", command=edit_metadata).grid(row=1, column=3)
    tk.Button(frame, text="Supprimer", command=delete_metadata).grid(row=2, column=3)
    tk.Button(frame, text="Ouvrir", command=open_image).grid(row=2, column=0)
    tk.Button(frame, text="Sauvegarder", command=save_image).grid(row=2, column=1)
    tk.Button(frame, text="Précédente", command=prev_image).grid(row=3, column=0)
    tk.Button(frame, text="Suivante", command=next_image).grid(row=3, column=1)

    def on_select(event):
        selection = listbox.curselection()
        if not selection:
            return
        index = selection[0]
        selected = listbox.get(index)
        key_value = selected.split(":", 1)
        if len(key_value) == 2:
            entry.delete(0, tk.END)
            entry.insert(0, key_value[1].strip())

    listbox.bind("<<ListboxSelect>>", on_select)

    if paths:
        image_paths = paths
        open_image(0)

    root.mainloop()

# Entrée
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Éditeur EXIF avec interface graphique.")
    parser.add_argument("images", nargs="*", help="Liste d'images à modifier (JPEG)")

    args = parser.parse_args()
    launch_gui(args.images if args.images else None)