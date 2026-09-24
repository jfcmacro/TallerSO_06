# Tuberías nombradas

Las tuberías en linux son anónimas y normalmente esta diseñadas para
comunicar procesos que tienen un tipo de relación padre-hijo o de
parientes.  Pero cuando los procesos no tienen relación directa, y
requieren un servicio, se requiere de otro tipo de tubería, estás son
las tuberías nombradas (**named pipes**).

Las tuberías nombredas se crean a través de la función
[`mkfifo(3)`](https://man7.org/linux/man-pages/man3/mkfifo.3.html). Esta
función recibe dos parámetros, el nombre de la tubería, que puede ser
relativo o absoluto, el usuario que crea la tubería debe tener
permisos para crear una tubería.