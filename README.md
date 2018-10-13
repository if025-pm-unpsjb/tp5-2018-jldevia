# mbed-blinky-makefile
Programa de prueba para la placa mbed LPC1768, que hace titilar uno de sus LEDs. El proyecto utiliza un Makefile, y esta basado en [mbed 2](https://developer.mbed.org/users/mbed_official/code/mbed/).

A continuación como se describe como importarlo, importarlo en Eclipse y ejecutarlo en la placa.

---

## Clonar el proyecto
Clonar el proyecto de prueba usando Git: https://github.com/unpsjb-rtsg/mbed-blinky-makefile
* Desde un cliente, usar la opción **[Clone]**.
* Desde la linea de comandos utilizar el comando `git clone`.

*Tip*: si tienen cuenta en GitHub, pueden probar con realizar un *fork* del proyecto.

---

## Importar el proyecto en Eclipse
Una vez clonado localmente, agregar el proyecto en Eclipse:
* Seleccionar **[File > Makefile project with existing code]**. 
* En la nueva ventana:
    * En **[Existing Code Location]** indicar el *path* al proyecto (usar el botón **[Browse...]**).
    * En **[Toolchain for Indexer]** seleccionar la opción *Cross ARM GCC* (¡importante!).

El proyecto debe aparecer ahora en la vista *Project Explorer*: 
* Hacer clic derecho sobre el mismo, y seleccionar **[Properties]** en el menú contextual.
* En la nueva ventana, seleccionar **[C/C++ Build > Settings]**. Si todo esta bien configurado, en la pestaña **[Toolchains]**, el campo *Name* debe indicar *GNU Tools for ARM Embedded Processors*.
* Hacer clic en **[Ok]**.

Verificar que dentro del proyecto exista el directorio `build`. En caso que no exista, crearlo haciendo clic derecho sobre el proyecto en la vista *Project Explorer*, y seleccionando **[New > Folder]** en el menú contextual.

---

## Compilar
Finalmente, para compilar el proyecto se puede:
* Hacer clic derecho sobre el proyecto en la vista *Project Explorer* y seleccionar **[Build]** en el menú contextual.
* Seleccionar en la barra de menúes de Eclipse **[Project > Build Project]**.
* Hacer clic en el ícono *Build* (un martillo).

Si el proyecto compilo correctamente, en la vista **[Console]** debe indicarse que se generó correctamente el archivo `build/blink.elf`.

---

## Ejecutar
Primero, verificar que el `pyocd-gdbserver` este correctamente configurado en Eclipse:
* Seleccionar **[Windows > Preferences]** en el menú de Eclipse.
* En la nueva ventana, seleccionar **[Run / Debug > PyOCD]** en la lista izquierda.
* En el campo *Executable* debe indicar `pyocd-gdbserver.exe` (sin la extensión en Linux o MacOS), y el campo *Folder* debe contener el *path* al ejecutable (en Windows `C:\Python27\Scripts`).
* Hacer clic en **[Ok]**.

A continuación se creará y ejecutará una configuración de *debug*:
* Seleccionar **[Run > Debug Configurations...]** en el menú de Eclipse.
* En la nueva ventana, hacer doble clic sobre **[GDB PyOCD debugging]** en el menú izquierdo. Esto crea una nueva configuración basada en este perfil.
* En el panel derecho:
    * En la pestaña **[Main]**, el campo *Project* debe indicar el nombre del proyecto de prueba. El campo *C/C++ Application* debe indicar el nombre del archivo ELF, en este caso `build/blink.elf`. Si no estuviera presente escribir el nombre del mismo, o bien hacer clic en el botón **[Search project]** o **[Explore]** para buscarlo.
    * En la pestaña **[Debugger]**, el campo *Executable* debe contener el valor `${pyocd_path}\${pyocd_executable}`. Estas dos variables son reemplazadas por los valores especificados el menú **[Run / Debug > PyOCD]**.
    * En la pestaña **[Common]**, seleccionar la opción **[Shared file:]**, indicando en el campo el nombre del proyecto. De esta manera el archivo `*.launch` con la configuración es guardado dentro del proyecto.
    * Hacer clic en el botón **[Apply]**.
* Con la placa mbed LPC1768 conectada a la PC, hacer clic en el botón **[Debug]**. Es posible que Eclipse pregunte si se desea cambiar a la perspectiva *Debug*, responder que sí.

