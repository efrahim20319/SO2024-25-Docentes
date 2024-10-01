echo "Criar xscript.bash" 

#Podemos optar por criar este xscript à parte...​
echo "#!/usr/bin/bash" > xscript.bash
echo "echo "'$$'": sleep "'$1' >> xscript.bash
echo "sleep "'$1' >> xscript.bash
echo "echo "'$$'" terminou" >> xscript.bash

chmod a+x xscript.bash

echo "Lançar 3 scripts"
./xscript.bash 3 &
./xscript.bash 4 &
./xscript.bash 5 &
echo "Esperar pelos 3 scripts"

wait

echo "Espera terminou"