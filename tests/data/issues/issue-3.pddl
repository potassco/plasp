;; PARC's modular printer domain
;; compiled into PDDL by Rong Zhou
;; To report a problem, send email to rzhou@parc.com
;; Papers:
;; 1. Planning for Modular Printers: Beyond Productivity
;;	Minh B. Do, Wheeler Ruml, and Rong Zhou. ICAPS'08
;; 2. On-line Planning and Scheduling: An Application to Controlling Modular Printers
;;	Minh B. Do, Wheeler Ruml, and Rong Zhou. AAAI'08
;; 3. On-line Planning and Scheduling for High-speed Manufacturing
;;	Wheeler Ruml, Minh B. Do, and Markus P.J. Fromherz. ICAPS'05
(define (domain upp)
(:requirements :typing :durative-actions)
(:types  size_t location_t side_t color_t image_t timepoint_t resource_t action_t sheet_t)
(:constants
		Letter - size_t

		Black
		Color - color_t

		Front
		Back - side_t

		Some_Feeder_Tray
		Some_Finisher_Tray
		EndCap_Entry-BlackContainer_Exit
		HtmOverBlack_Entry-EndCap_Exit
		HtmOverBlack_Exit-Down_TopEntry
		ColorContainer_Entry-Down_BottomExit
		ColorContainer_ExitToIME-ColorPrinter_Entry
		ColorPrinter_Exit-ColorContainer_EntryFromIME
		ColorContainer_Exit-Up_BottomEntry
		Down_BottomEntry-ColorFeeder_Exit
		BlackContainer_Entry-BlackFeeder_Exit
		Down_TopExit-HtmOverColor_Entry
		HtmOverColor_Exit-Up_TopEntry
		BlackContainer_ExitToIME-BlackPrinter_Entry
		BlackPrinter_Exit-BlackContainer_EntryFromIME
		Finisher1_Entry-Up_TopExit
		Finisher2_Entry-Finisher1_Exit
		Finisher1_Tray
		Finisher2_Exit
		Finisher2_Tray - location_t

		ColorContainer-ToIME-Letter-T0
		ColorPrinter-Simplex-Letter-T0
		ColorPrinter-SimplexMono-Letter-T0
		ColorFeeder-Feed-Letter-T0
		BlackFeeder-Feed-Letter-T0
		Down-MoveBottom-Letter-T1
		Down-MoveDown-Letter-T1
		BlackPrinter-Simplex-Letter-T0
		BlackPrinter-SimplexAndInvert-Letter-T0
		Up-MoveTop-Letter-T1
		Up-MoveUp-Letter-T1
		Finisher1-Stack-Letter-T0
		Finisher2-Stack-Letter-T0 - timepoint_t

		ColorContainer_Roller-RSRC
		ColorPrinter_Drum-RSRC
		ColorFeeder_ExitNip-RSRC
		BlackFeeder_ExitNip-RSRC
		Down_BottomExitNip-RSRC
		BlackPrinter_Drum-RSRC
		Up_TopExitNip-RSRC
		Finisher1_EntryNip-RSRC
		Finisher2_EntryNip-RSRC - resource_t

		ColorContainer-ToIME-Letter
		ColorPrinter-Simplex-Letter
		ColorPrinter-SimplexMono-Letter
		ColorFeeder-Feed-Letter
		BlackFeeder-Feed-Letter
		Down-MoveBottom-Letter
		Down-MoveDown-Letter
		BlackPrinter-Simplex-Letter
		BlackPrinter-SimplexAndInvert-Letter
		Up-MoveTop-Letter
		Up-MoveUp-Letter
		Finisher1-Stack-Letter
		Finisher2-Stack-Letter - action_t
)
(:predicates
		(Sheetsize ?sheet - sheet_t ?size - size_t)
		(Location ?sheet - sheet_t ?location - location_t)
		(Hasimage ?sheet - sheet_t ?side - side_t ?image - image_t)
		(Sideup ?sheet - sheet_t ?side - side_t)
		(Stackedin ?sheet - sheet_t ?location - location_t)
		(Imagecolor ?image - image_t ?color - color_t)
		(Notprintedwith ?sheet - sheet_t ?side - side_t ?color - color_t)
		(Oppositeside ?side1 - side_t ?side2 - side_t)
		(Timepoint ?sheet - sheet_t ?timepoint - timepoint_t)
		(Hasallrsrcs ?sheet - sheet_t ?action - action_t)
		(Available ?resource - resource_t)
		(Prevsheet ?sheet1 - sheet_t ?sheet2 - sheet_t)
		(Uninitialized)

)
)


