
�ste proyecto requiere:
- compilador C18 (no sirve el XC8!!!)
- librar�a Microchip MLA, versi�n LEGACY! (la lib. actual no tiene soporte para ethernet)


Archivos HTTPPrint.h/.idx y control_html.c son auto-generados por la utilidad Microchip MPFS Generator
TCPIPConfig.h est� configurado con utilidad Microchip MLA TCPIP Configuration Wizard

Hardware Profile: Configurado "a mano", bas�ndose en ejemplos de los demos de Microchip MLA y en los pines utilizados para los perif�ricos.

WebPages/: Contiene los archivos html/css/js necesarios para el servidor http. tales archivos son convertidos con el MMPFSGenerator.

bender_face.X: Configs. espec�ficas de MPLAB

bender_face.h/.c: main del programa

CustomHTTPApp: Implementaci�n del servidor Web, en particular, se preocupa de manejar los pedidos HTTP-GET para el control del rostro y dar feedback del estado actual, as� como de los datos de la placa.

dudas a: matias pavez - matias.pavez.b@gmail.com