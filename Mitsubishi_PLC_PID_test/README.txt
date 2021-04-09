--------------------------------------------------------------------------
-----------------   GX WORKS PID FUNKSJONSBLOKK   ------------------------
--------------------------------------------------------------------------

Det ligger ved to PID funksjonsblokker:

1.    navn:   "PID_controller"
      Dette er en simpel PID regulator.
      Inputs:   PV        (Process Value)
                SP        (Setpoint / referanse)
                Kp        (Justering av P-ledd)
                Ti        (Justering av I-ledd)
                Kd        (Justering av D-ledd)
                n_filter  (Filterkonstant, normalt 10.0)
                SampleTime (Tastetiden)
                
      Outputs:  MV        (Modulated Value, Pådrag ut fra regulator)
      
      
2.    navn:     "PID_controller_v2"
      Likt som "PID_controller" men har i tillegg muligheten for å kjøre MANUELT/AUTO.
      

3.    navn:     "PID_controller_v3"
      Likt som "PID_controller" men har i tillegg muligheten for Tracking.
      Dette gjør det mulig å ha flere PID-regulatorer parallelt.
      Kan også ha andre filtre eller manuell styring paralellt vha. Tracking input. 
      
