<AnimDB FragDef="Animations/YBot/ADB/YBotFragmentIds.xml" TagDef="Animations/YBot/ADB/YBotTags.xml">
 <FragmentList>
  <Movement>
   <Fragment BlendOutDuration="0.2" Tags="">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.00011530519"/>
     <Animation name="WalkCycle" flags="Loop"/>
    </AnimLayer>
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2" flags="TimeWarping"/>
     <Animation name="YBot_Aimpose" flags="Loop+ForceSkelUpdate"/>
    </AnimLayer>
    <ProcLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.30000001"/>
     <Procedural type="AimPose">
      <ProceduralParams CryXmlVersion="2" Animation="" BlendTime="1" AnimationLayer="14"/>
     </Procedural>
    </ProcLayer>
   </Fragment>
  </Movement>
  <AimPose>
   <Fragment BlendOutDuration="0.2" Tags="">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2" flags="TimeWarping"/>
     <Animation name="YBot_AimposeOmniDir" flags="Loop+ForceSkelUpdate"/>
    </AnimLayer>
   </Fragment>
  </AimPose>
 </FragmentList>
</AnimDB>
