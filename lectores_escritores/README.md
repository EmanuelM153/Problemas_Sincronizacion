# Lectores y escritores

En esta solución, se da prioridad a los procesos lectores, permitiendo que múltiples lectores puedan acceder al recurso de manera concurrente.

Mecanismos de Implementación:
- Se utiliza un contador que rastrea el número de lectores activos.
- Si hay lectores activos, los nuevos lectores pueden seguir accediendo al recurso sin interrupción.
- Sin embargo, si un escritor está esperando, debe esperar hasta que todos los lectores hayan terminado, lo cual puede provocar inanición de escritores si hay un flujo constante de lectores.
