
Éste proyecto requiere:
- compilador C18 (no sirve el XC8!!!)
- libraría Microchip MLA, versión LEGACY! (la lib. actual no tiene soporte para ethernet)


Archivos HTTPPrint.h/.idx y control_html.c son auto-generados por la utilidad Microchip MPFS Generator
TCPIPConfig.h está configurado con utilidad Microchip MLA TCPIP Configuration Wizard

Hardware Profile: Configurado "a mano", basándose en ejemplos de los demos de Microchip MLA y en los pines utilizados para los periféricos.

WebPages/: Contiene los archivos html/css/js necesarios para el servidor http. tales archivos son convertidos con el MMPFSGenerator.

bender_face.X: Configs. específicas de MPLAB

bender_face.h/.c: main del programa

CustomHTTPApp: Implementación del servidor Web, en particular, se preocupa de manejar los pedidos HTTP-GET para el control del rostro y dar feedback del estado actual, así como de los datos de la placa.

dudas a: matias pavez - matias.pavez.b@gmail.com