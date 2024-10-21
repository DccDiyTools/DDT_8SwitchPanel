#!/bin/bash

export PCBDRAW_LIB_PATH=$KIBOT_DIR/../
rm -rf doc
CURRENT=`pwd`


PRO_FILES=("*.kicad_pro")
#echo $PRO_FILES
if [ ${#PRO_FILES[@]} == 1 ];then
    BOARD=$(echo $PRO_FILES)
    BOARD="${BOARD%.*}"
else 
    echo "Too many ${#PRO_FILES[@]} kicad_pro files. "
    exit 0
    #If not found change manually.
    #BOARD=CHANGE_ME
fi

#FILES="00_erc_drc.kibot.yaml 01_pdf_doc.kibot.yaml 02_build_doc.kibot.yaml 03_graphic_doc.kibot.yaml 04_fab_board.kibot.yaml 05_fab_pcba_jlcpcb.kibot"
FILES="00_erc_drc.kibot.yaml 01_pdf_doc.kibot.yaml 02_build_doc.kibot.yaml 03_graphic_doc.kibot.yaml 04_fab_board.kibot.yaml"  

#FILES="02_build_doc.kibot.yaml"  
#FILES=""

for i in $FILES 
do
    echo "Running $i on $BOARD"
    kibot -e $BOARD.kicad_sch -b $BOARD.kicad_pcb -d doc -c $KIBOT_ROOT_SCRIPTS/$i $@
done 

if [ -f manual.kibot.yaml ]; 
then
    kibot -e $BOARD.kicad_sch -b $BOARD.kicad_pcb -d manual -c manual.kibot.yaml $@
fi

rm -f doc/*.ogv

exit 0