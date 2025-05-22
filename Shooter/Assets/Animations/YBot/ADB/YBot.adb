<AnimDB FragDef="Animations/YBot/ADB/YBotFragmentIds.xml" TagDef="Animations/YBot/ADB/YBotTags.xml">
 <FragmentList>
  <Movement>
   <Fragment BlendOutDuration="0.2" Tags="">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.00011530519" flags="TimeWarping"/>
     <Animation name="WalkCycle" flags="Loop+ForceSkelUpdate" speed="2"/>
    </AnimLayer>
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0" flags="TimeWarping"/>
     <Animation name="YBot_AimposeOmniDir" flags="Loop+ForceSkelUpdate" speed="20"/>
    </AnimLayer>
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
