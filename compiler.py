import subprocess 

file_name = "app.exe"
include_dir = "dependencies/include"
lib_dir = "dependencies/lib"
lib = "-lglfw3 -lglew32s -lopengl32 -lgdi32"

preprocessor = "-DGLEW_STATIC"

def main():
    subprocess.call(f"g++ -g {preprocessor} -c main.cpp functions.cpp -I {include_dir}")
    subprocess.call(f"g++ -g {preprocessor} *.o -o {file_name} -L {lib_dir} {lib}")

if "__main__" == __name__:
    main()