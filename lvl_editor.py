import bpy

# Set the file path and name for the output text file
wall_file = "C:/Users/Simon/Desktop/Studium/Programmieren/Programmieren 2/s2_3D_game/walls.txt"
sector_file = "C:/Users/Simon/Desktop/Studium/Programmieren/Programmieren 2/s2_3D_game/sectors.txt"
object_file = "C:/Users/Simon/Desktop/Studium/Programmieren/Programmieren 2/s2_3D_game/objects.txt"
wall_start = 0
wall_end = -1
# Open the output file for writing
with open(wall_file, "w") as wfile:
    with open(sector_file, "w") as sfile:
        with open(object_file, "w") as ofile:
        # Loop through the edges and write the start and end points to the file
            for obj in bpy.context.scene.objects:
                if obj.name.split(":")[0] == "sec":
                    edges = obj.data.edges
                    z1 = int(obj.name.split(":")[1])
                    z2 = int(obj.name.split(":")[2])
                    tid = int(obj.name.split(":")[3])
                    isFloor = int(obj.name.split(":")[4])
                    for e in edges:
                        start = obj.data.vertices[e.vertices[0]].co * 100
                        end = obj.data.vertices[e.vertices[1]].co * 100
                        wfile.write(f"{int(start[0])},{int(start[1])},{int(end[0])},{int(end[1])}\n")
                        wall_end += 1
                    sfile.write(f"{int(wall_start)},{int(wall_end)},{int(z1)},{int(z2)},{int(tid)},{int(isFloor)}\n")
                    wall_start = wall_end + 1
                
                if obj.name.split(":")[0] == "obj":
                    position = obj.location
                    xpos = position[0] * 100
                    ypos = position[1] * 100
                    zpos = obj.name.split(":")[1]
                    texture_id = obj.name.split(":")[2]
                    scale = float(obj.name.split(":")[3]) * 0.1
                    ofile.write(f"{int(xpos)},{int(ypos)},{int(zpos)},{int(texture_id)},{float(scale)}\n")
                
            
            
            
