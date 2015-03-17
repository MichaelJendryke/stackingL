#!/bin/tcsh -f
rm -r SBAsubsets
set WORKDIR = $cwd
mkdir SBAsubsets
set SB_DIR = $WORKDIR/SBAsubsets
set masterdate = `\ls | gawk '{print substr($0,1,8)}' | head -1`
echo $masterdate
set firstdir = `\ls | gawk '{print substr($0,1,17)}' | head -1`
set first = `grep 'First_pixel ' $WORKDIR/$firstdir/master.res | gawk 'END {print $4}'`
set last  = `grep 'Last_pixel '  $WORKDIR/$firstdir/master.res | gawk 'END {print $4}'`
@ width = ($last - $first) + 1

echo $width


foreach pair(`cat combinations.csv`)
   set master = `echo $pair | cut -c1-8`
   set slave  = `echo $pair | cut -c10-17`
   echo "------------" $master " & " $slave
   set IFG_DIR = $SB_DIR/$master"_"$slave
  
    if (! -e $IFG_DIR) then
        mkdir $IFG_DIR
	mkdir $IFG_DIR/Outdata
	mkdir $IFG_DIR/Inputfiles
    endif
    
##########################
# SBAS Master is SM Master
    if ($master == $masterdate) then
        echo "The master from small baselines list is the same as the master from SM"
        echo "master " $master " == masterdate " $masterdate
        
        cp $WORKDIR/$firstdir/master.res  $IFG_DIR/master.res
        
        echo "s#Outdata/master_ovs.raw#"$WORKDIR"/"$firstdir"/Outdata/"master_ovs.raw"#"  > $IFG_DIR/master_tmp_sed.in
        sed -f $IFG_DIR/master_tmp_sed.in $IFG_DIR/master.res                                       >> $IFG_DIR/mastertest.res
        rm  -f $IFG_DIR/master_tmp_sed.in
        rm -f  $IFG_DIR/master.res
        mv $IFG_DIR/mastertest.res $IFG_DIR/master.res

##########################
# SBAS Master is SM Slave
    else
        echo "The master from small baselines list is a slave in SM"
        echo "Creating a master.res from a slave.res"
        #echo "master " $master " != masterdate " $masterdate
        
        sed -n '1,/_Start_crop:/p' $WORKDIR/$masterdate"_"$master/slave.res            > $IFG_DIR/tmp_master.res
        sed -e '/resample/d' $IFG_DIR/tmp_master.res                                   > $IFG_DIR/mod_master.res
        rm -f $IFG_DIR/tmp_master.res
        mv $IFG_DIR/mod_master.res $IFG_DIR/tmp_master.res
        
        echo "******************************************************************* "   >> $IFG_DIR/tmp_master.res 
        echo "Data_output_file: " $WORKDIR"/"$masterdate"_"$master"/Outdata/"slave.resampled  >> $IFG_DIR/tmp_master.res 
        echo "Data_output_format:         complex_real4"                              >> $IFG_DIR/tmp_master.res
        grep '(w.r.t. original_image):' $WORKDIR/$masterdate"_"$master/master.res     >> $IFG_DIR/tmp_master.res
        echo "******************************************************************* "   >> $IFG_DIR/tmp_master.res 
        sed -n '/* End_crop:_NORMAL/,/* End_oversample:_NORMAL/p' $WORKDIR/$masterdate"_"$master/slave.res >> $IFG_DIR/tmp_master.res
        echo "******************************************************************* "   >> $IFG_DIR/tmp_master.res 
        echo " "                                                                      >> $IFG_DIR/tmp_master.res
       
        echo "s# SLAVE RESULTFILE:         slave.res# SLAVE RESULTFILE:         slave.res IS NOW master.res for "$master" and "$slave"#"  > $IFG_DIR/tmp_sed.in
        sed -f $IFG_DIR/tmp_sed.in $IFG_DIR/tmp_master.res                            > $IFG_DIR/mod_master.res
        rm  -f $IFG_DIR/tmp_sed.in
        mv $IFG_DIR/mod_master.res $IFG_DIR/tmp_master.res

        echo "s#Outdata/slave_ovs.raw#"$WORKDIR"/"$masterdate"_"$master"/Outdata/"slave.resampled"#"  > $IFG_DIR/tmp_sed.in
        sed -f $IFG_DIR/tmp_sed.in $IFG_DIR/tmp_master.res                            > $IFG_DIR/mod_master.res
        rm  -f $IFG_DIR/tmp_sed.in
        mv $IFG_DIR/mod_master.res $IFG_DIR/master.res
 
    endif

##########################
# SBAS Slave is SM Master
    if ($slave == $masterdate) then
        echo "The slave from small baselines list is the master in SM" 
        #echo "slave " $slave " == masterdate " $masterdate
        cp $WORKDIR/$firstdir/master.res $IFG_DIR/master.res

        echo "s#Outdata/master_ovs.raw#"$WORKDIR"/"$firstdir"/Outdata/"master_ovs.raw"#" > $IFG_DIR/tmp_sed.in
        sed -f $IFG_DIR/tmp_sed.in $IFG_DIR/master.res                           >> $IFG_DIR/mod_master.res
        rm  -f $IFG_DIR/master_tmp_sed.in
        rm -f  $IFG_DIR/master.res
        mv $IFG_DIR/mod_master.res $IFG_DIR/tmp_master.res
        
        echo "s# MASTER RESULTFILE:        master.res# MASTER RESULTFILE:        master.res IS still master.res for "$master" and "$slave"#"  > $IFG_DIR/tmp_sed.in
        sed -f $IFG_DIR/tmp_sed.in $IFG_DIR/tmp_master.res                            > $IFG_DIR/mod_master.res
        rm  -f $IFG_DIR/tmp_sed.in
        mv $IFG_DIR/mod_master.res $IFG_DIR/master.res


        set pslave = $master
    
##########################
# SBAS Slave is SM Slave
    else
        echo "The slave from small baselines list is a slave in SM too" 
        #echo "slave " $slave " != masterdate " $masterdate
        set pslave = $slave
    endif
      
    echo "pslave: " $pslave
    sed -n '1,/_Start_crop:/p' $WORKDIR/$masterdate"_"$pslave/slave.res                       > $IFG_DIR/tmp_slave.res
    echo "******************************************************************* "              >> $IFG_DIR/tmp_slave.res 
    echo "Data_output_file:           Outdata/slave.resampled" >> $IFG_DIR/tmp_slave.res #$WORKDIR"/"$masterdate"_"$pslave"/Outdata/"slave.resampled     >> $IFG_DIR/tmp_slave.res 
    echo "Data_output_format:         complex_real4 "                                        >> $IFG_DIR/tmp_slave.res 
    grep '(w.r.t. original_image):' $WORKDIR/$masterdate"_"$pslave/master.res                 >> $IFG_DIR/tmp_slave.res
    echo "******************************************************************* "              >> $IFG_DIR/tmp_slave.res 
    sed -n '/End_crop:/,/End_resample/p' $WORKDIR/$masterdate"_"$pslave/slave.res            >> $IFG_DIR/tmp_slave.res
    echo "******************************************************************* "              >> $IFG_DIR/tmp_slave.res 
    echo " "                                                                                 >> $IFG_DIR/tmp_slave.res 
    
    set SKIP_FILTAZI = 1
    if ($SKIP_FILTAZI == 0) then
        echo "s#slave_res.slc#slave_azifilt.slc#" > $WORKDIR/tmp_sed.in
    else
        echo "s#Outdata/slave.resampled#"$WORKDIR"/"$masterdate"_"$pslave"/Outdata/"slave.resampled"#" > $WORKDIR/tmp_sed.in
    endif
    sed -f $WORKDIR/tmp_sed.in $IFG_DIR/tmp_slave.res >> $IFG_DIR/slave.res 
    rm -f $IFG_DIR/tmp_slave.res

   

    echo "s# SLAVE RESULTFILE:         slave.res# SLAVE RESULTFILE:         slave.res IS still slave.res for "$master" and "$slave"#"  > $IFG_DIR/tmp_sed.in
    sed -f $IFG_DIR/tmp_sed.in $IFG_DIR/slave.res                            > $IFG_DIR/mod_slave.res
    rm  -f $IFG_DIR/tmp_sed.in
    mv $IFG_DIR/mod_slave.res $IFG_DIR/slave.res
    


    
    sed -n '1,/comp_coregpm:		1/p' $WORKDIR/$masterdate"_"$pslave/master_slave.res > $IFG_DIR/interferogram.out
    echo "interfero:                    0"                                                  >> $IFG_DIR/interferogram.out
    echo "coherence:                    0"                                                  >> $IFG_DIR/interferogram.out
    echo "comp_refphase:				0"                                                  >> $IFG_DIR/interferogram.out
    echo "subtr_refphase:				0"                                                  >> $IFG_DIR/interferogram.out
    echo "comp_refdem:                  0"                                                  >> $IFG_DIR/interferogram.out
    echo "subtr_refdem:                 0"                                                  >> $IFG_DIR/interferogram.out
    echo "filtphase:                    0"                                                  >> $IFG_DIR/interferogram.out
    echo "unwrap:						0"                                                  >> $IFG_DIR/interferogram.out
    #echo "est_orbits:                   0"                                                  >> $IFG_DIR/interferogram.out
    echo "slant2h:                      0"                                                  >> $IFG_DIR/interferogram.out
    echo "geocoding:                    0"                                                  >> $IFG_DIR/interferogram.out
    echo "dinsar:						0"                                                  >> $IFG_DIR/interferogram.out
    echo "NOT_USED2:                    0"                                                  >> $IFG_DIR/interferogram.out
    sed -n '/End_process_control/,/* End_comp_coregpm:_NORMAL/p' $WORKDIR/$masterdate"_"$pslave/master_slave.res  >> $IFG_DIR/interferogram.out
    echo "******************************************************************* "                                   >> $IFG_DIR/interferogram.out
    echo " "                                                                                                      >> $IFG_DIR/interferogram.out

	#Copy inputfiles
    cd $IFG_DIR 
    echo "Processing in Doris now!"
        
	cp $WORKDIR/$firstdir/Inputfiles/interfero                              $IFG_DIR/Inputfiles/interfero
    # cp $WORKDIR/$firstdir/Inputfiles/interfero                              $IFG_DIR/interfero
    # echo "s#Outdata/master_slave.cint#cint.raw#" > $IFG_DIR/tmp_sed.in
    # sed -f $IFG_DIR/tmp_sed.in $IFG_DIR/interfero > $IFG_DIR/mod_interfero
    # rm  -f $IFG_DIR/tmp_sed.in
    # mv $IFG_DIR/mod_interfero                                               $IFG_DIR/Inputfiles/interfero

    cp $WORKDIR/$firstdir/Inputfiles/comprefpha                             $IFG_DIR/Inputfiles/comprefpha
	
	cp $WORKDIR/$firstdir/Inputfiles/subtrrefpha                            $IFG_DIR/Inputfiles/subtrrefpha
    # cp $WORKDIR/$firstdir/Inputfiles/subtrrefpha                            $IFG_DIR/subtrrefpha
    # echo "s#Outdata/master_slave.srp#master_slave.srp#" > $IFG_DIR/tmp_sed.in
    # sed -f $IFG_DIR/tmp_sed.in $IFG_DIR/subtrrefpha > $IFG_DIR/mod_subtrrefpha
    # rm  -f $IFG_DIR/tmp_sed.in
    # mv $IFG_DIR/mod_subtrrefpha                                             $IFG_DIR/Inputfiles/subtrrefpha

    cp $WORKDIR/$firstdir/Inputfiles/comprefdem                             $IFG_DIR/Inputfiles/comprefdem

	cp $WORKDIR/$firstdir/Inputfiles/subtrrefdem                            $IFG_DIR/Inputfiles/subtrrefdem
    # cp $WORKDIR/$firstdir/Inputfiles/subtrrefdem                            $IFG_DIR/subtrrefdem
    # echo "s#Outdata/cint.minrefdem.raw#cint.minrefdem.raw#" > $IFG_DIR/tmp_sed.in
    # sed -f $IFG_DIR/tmp_sed.in $IFG_DIR/subtrrefdem > $IFG_DIR/mod_subtrrefdem
    # rm  -f $IFG_DIR/tmp_sed.in
    # mv $IFG_DIR/mod_subtrrefdem                                             $IFG_DIR/Inputfiles/subtrrefdem

	cp $WORKDIR/$firstdir/Inputfiles/coherence                              $IFG_DIR/Inputfiles/coherence
    # cp $WORKDIR/$firstdir/Inputfiles/coherence                              $IFG_DIR/coherence
    # echo "s#Outdata/master_slave.coh#coh_4l.raw#" > $IFG_DIR/tmp_sed.in
    # sed -f $IFG_DIR/tmp_sed.in $IFG_DIR/coherence > $IFG_DIR/mod_coherence
    # rm  -f $IFG_DIR/tmp_sed.in
    # mv $IFG_DIR/mod_coherence                                               $IFG_DIR/Inputfiles/coherence


	#Start to process in doris   
    doris $IFG_DIR/Inputfiles/interfero
    #doris $IFG_DIR/Inputfiles/comprefpha
    #doris $IFG_DIR/Inputfiles/subtrrefpha
    #doris $IFG_DIR/Inputfiles/comprefdem
    #doris $IFG_DIR/Inputfiles/subtrrefdem
    doris $IFG_DIR/Inputfiles/coherence
  







    if ($master == $masterdate) then
        #cpxsum cint.raw $WORKDIR/$masterdate"_"$pslave/Outdata/master_slave.cint tmp_cint3.raw $width cr4 -1 1
        #cpxsum tmp_cint3.raw $WORKDIR/$masterdate"_"$pslave/Outdata/cint.minrefdem.raw cint.minrefdem.raw $width cr4 1 1
    else if ($slave == $masterdate) then
        #cpxsum $WORKDIR/$masterdate"_"$pslave/Outdata/master_slave.cint cint.raw tmp_cint.raw $width cr4 -1 1
        #cpxsum tmp_cint.raw $WORKDIR/$masterdate"_"$pslave/Outdata/cint.minrefdem.raw cint.minrefdem.raw $width cr4 -1 1
    else
        #cpxsum cint.raw $WORKDIR/$masterdate"_"$pslave/Outdata/master_slave.cint tmp_cint.raw $width cr4 1 1
        #cpxsum tmp_cint.raw $WORKDIR/$masterdate"_"$pslave/Outdata/cint.minrefdem.raw tmp_cint2.raw $width cr4 -1 1
        #cpxsum tmp_cint2.raw $WORKDIR/$masterdate"_"$pslave/Outdata/master_slave.cint tmp_cint3.raw $width cr4 -1 1
        #cpxsum tmp_cint3.raw $WORKDIR/$masterdate"_"$pslave/Outdata/cint.minrefdem.raw cint.minrefdem.raw $width cr4 1 1
    endif


    #rm -f tmp_cint*.raw
    #rm -f cint.raw
    #rm -f *azifilt.slc
    #set endras = l.ras
    #set endraw = l.raw
    #cpxfiddle -w$width -fcr4 -qmixed -M1/1 -osunraster -e0.2 -s1.8 cint.minrefdem.raw > cint.minrefdem_1$endras
    #cpxfiddle -w$width -fcr4 -qnormal -M1/1 -ofloat cint.minrefdem.raw > cint.minrefdem_1$endraw




end

rm -f tmp_sed.in
cd $WORKDIR
