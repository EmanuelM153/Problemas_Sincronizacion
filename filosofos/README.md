# La cena de los filósofos

La solución propuesta consiste en que cada filósofo comienza con una prioridad asignada aleatoriamente de un conjunto de n prioridades diferentes. Además, cada filósofo decide de forma aleatoria cuál tenedor tomar primero (el de la izquierda o el de la derecha). Si un filósofo no puede obtener ambos tenedores, su prioridad aumenta, lo que le dará una mayor probabilidad de obtener los tenedores en su próximo intento. Luego, suelta cualquier tenedor que haya tomado y espera por un tiempo, que depende de su prioridad, antes de intentar nuevamente. Si consigue ambos tenedores, procede a comer y luego suelta ambos tenedores. Este enfoque dinámico de prioridades ayuda a evitar la inanición y mejora la equidad en la asignación de los recursos. 

# Compilación
Para la compilación es necesario contar con la librería ncurses

# Mejoras futuras
- Manejar mejor la muestra de más filósofos en la pantalla
- Probar otras formas de implementar la función calcularTiempoEspera
