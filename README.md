# prog2-tp2

TPN°2 C

# Compilar

Para compilar el programa,

\$ gcc \_\_main.c

# Ejecutar

\$ fileName arg1 arg2 arg3

# Argumentos

- arg1 = Path al archivo de ciudades
- arg2 = Path al archivo de personas
- arg3 = Path al archivo de salida

# Utilidad

Este programa toma 3 archivos:

- Archivo de ciudades
- Archivo de personas
- Archivo de salida

Luego pide un numero de personas deseada, digamos x
y en el archivo de salida escribe x personas tomadas
aleatoriamente del archivo de personas, con formato

NOMBRE,APELLIDO,CIUDAD,EDAD,GENERO,GENERO_DE_INTERES

La ciudad será el nombre que le corresponda al código
escrito en el archivo de personas. Al igual que el
género y el género de interes.

Ejemplos de los archivos de entrada se pueden encontrar
en la carpeta db

# Boceto de funcionamiento

Recibo la cantidad de personas pedidas, llamese p

Tomo con una chance del 40% algunas personas, supongamos, m personas.
Cuento la cantidad de personas totales del archivo, supongamos n personas.

Ahora si m < p,
genero p-m numeros aleatorios, que no esten en r,
siendo r el conjunto de personas seleccionadas anteriormente.

Una vez generado el conjunto nuevo de numeros,
recorro una vez mas el archivo para conseguir las personas faltantes.

A la vez que voy cargando las personas, busco la ciudad de codigo mas alto
y lo guardo en @max_city, estas serán el total de ciudades que necesitaré entrar.

Leo las @max_city ciudades.

Escribo los archivos de salida

#######
Este approach sirve para ~10^4 ciudades y ~4\*10^4 personas
