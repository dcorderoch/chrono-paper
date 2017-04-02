# Estructura

usando rutas (estilo UNIX) relativas, a la ruta de éste archivo

en `../Documentation`, se encuentra un archivo en LaTeX con la documentación en prosa, en formato iEEE, compsoc, obtenido del sitio web de la  [iEEE](https://www.ieee.org/publications_standards/publications/authors/author_templates.html)

en `../Documentos`, se encuentra un archivo en MarkDown describiendo los avances semanales

en `.` se encuentra éste documento y una carpeta `src`, conteniendo el código fuente de [PintOS](http://web.stanford.edu/class/cs140/projects/pintos/pintos.html)

en `../thread` se encuentra el código fuente de la aplicación usada para probar los algoritmos con múltiples hilos de ejecución

## en el código fuente de PintOS

en `src/threads` se encuentra el código fuente del kernel de pintos

en `src/userprog` se encuentra el código fuente para el cargador de los programas de usuario

en `src/vm` se encuentran makefiles para compilar código de memoria virtual

en `src/filesys` se encuentra el código fuente para un sistema de archivos básico

en `src/devices` se encuentra el código fuente para E/S; teclado, timer, discos, tc.

en `src/lib` se encuentra una implementación de un sub-conjunto de la biblioteca estándar de C

en `src/lib/kernel` están las partes que sólamente usa el kernel, no los programas de usuario

en `src/lib/user` están las partes que sólamente se usan en los programas de usuario

en `src/misc` se encuentran algunos archivos que pueden ser útiles para compilar desde un sistema con diferente arquitectura, o correr el sistema en una máquina virtual (usando bochs)

en `src/utils` se encuentran archivos que pueden resultar útiles al usar pintos directamente en una máquina (no en una máquina virtual)

### en `src/threads`

`loader.S` y `loader.h` cargan el kernel, al enlazarlo, se convierten en un archivo de 215 bytes, que se cargan en el BIOS y que cargan el kernel del disco a memoria, y "salta" a start() en start.S

`start.S` se encarga de la configuración básica para la protección de memoria y las operaciones de CPUs 80x86 (es parte del kernel)

"kernel.lds.S" es el script para enlazar el kernel, establece la dirección en memoria del kernel y pone start.S cerca del principio de la imagen del kernel.

`init.c` y `init.h` se encargan de la inicialización del kernel, incluyendo el "programa principal" del kernel

`thread.c` y `thread.h` se encargan del soporte básico de los hilos de ejecución

`switch.S` y `switch.h` se encargan del cambio de los hilos de ejecución

`palloc.c` y `palloc.h` se encargan de asignar páginas (paginación), que son de 4kB

`malloc.c` y `malloc.h` se encargan de la implementación de malloc() y free() para el kernel

`interrupt.c`y `interrupt.h` se encargan del manejo de las interrupciones, además de funciones para encender y apagar interrupciones

"intr-stubs.S" y "intr-stubs.h" se encargan del manejo a bajo nivel de las interrupciones

`synch.c` y `synch.h` definen las primitivas de sincronización (semáforos, cerraduras (locks), variables condicionales, etc.

`io.h` tiene funciones para E/S (puertos)

`vaddr.h` y `pte.h` incluyen funciones y macros para trabajar con direcciones virtuales y tablas de paginación

`flags.h` contiene macros que definien algunos bits en el registro de "flags"
