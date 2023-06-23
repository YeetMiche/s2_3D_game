import bpy

# Set the file path and name for the output text file
wall_file = "C:/Users/maste/Desktop/Studium/Programmieren/Programmieren 2/s2_3D_game/walls.txt"
sector_file = "C:/Users/maste/Desktop/Studium/Programmieren/Programmieren 2/s2_3D_game/sectors.txt"
wall_start = 0
wall_end = 0
# Open the output file for writing
with open(wall_file, "w") as wfile:
    with open(sector_file, "w") as sfile:
        # Loop through the edges and write the start and end points to the file
        for obj in bpy.context.scene.objects:
            edges = obj.data.edges
            z1 = int(obj.name.split(":")[0])
            z2 = int(obj.name.split(":")[1])
            for e in edges:
                start = obj.data.vertices[e.vertices[0]].co * 100
                end = obj.data.vertices[e.vertices[1]].co * 100
                wfile.write(f"{int(start[0])},{int(start[1])},{int(end[0])},{int(end[1])}\n")
                wall_end += 1
            sfile.write(f"{int(wall_start)},{int(wall_end)},{int(z1)},{int(z2)}\n")
            wall_start = wall_end