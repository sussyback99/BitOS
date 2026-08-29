import os

def build_bitos():
    if os.system("nasm -f bin boot.asm -o boot.bin") != 0:
        return

    c_flags = "-m32 -ffreestanding -fno-exceptions -fno-rtti -c -O2"
    if os.system(f"g++ {c_flags} kernel.cpp -o kernel.o") != 0:
        return

    link_flags = "-m elf_i386 -Ttext 0x10000 --oformat binary"
    if os.system(f"ld {link_flags} kernel.o -o kernel.bin") != 0:
        return

    try:
        with open("boot.bin", "rb") as f_boot:
            boot_data = f_boot.read()
        with open("kernel.bin", "rb") as f_kernel:
            kernel_data = f_kernel.read()
            
        with open("bitos.bin", "wb") as f_out:
            f_out.write(boot_data)
            f_out.write(kernel_data)
    except:
        return

if __name__ == "__main__":
    build_bitos()
