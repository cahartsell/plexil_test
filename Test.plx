<?xml version="1.0" encoding="UTF-8"?>
<PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:tr="extended-plexil-translator"
            FileName="/home/charles/work/plexil-4.0.1/examples/basic/plexil_test/Test.ple">
   <GlobalDeclarations LineNo="2" ColNo="0">
      <CommandDeclaration LineNo="2" ColNo="0">
         <Name>debugMsg</Name>
         <Parameter>
            <Name>msg</Name>
            <Type>String</Type>
         </Parameter>
      </CommandDeclaration>
      <CommandDeclaration LineNo="3" ColNo="0">
         <Name>driveToNextWaypoint</Name>
      </CommandDeclaration>
      <CommandDeclaration LineNo="4" ColNo="0">
         <Name>reverseAndTurn</Name>
      </CommandDeclaration>
      <CommandDeclaration LineNo="5" ColNo="0">
         <Name>dock</Name>
      </CommandDeclaration>
      <CommandDeclaration LineNo="6" ColNo="0">
         <Name>update</Name>
      </CommandDeclaration>
      <StateDeclaration LineNo="8" ColNo="8">
         <Name>at_waypoint</Name>
         <Return>
            <Name>_return_0</Name>
            <Type>Boolean</Type>
         </Return>
      </StateDeclaration>
      <StateDeclaration LineNo="9" ColNo="8">
         <Name>tilt_sensor</Name>
         <Return>
            <Name>_return_0</Name>
            <Type>Boolean</Type>
         </Return>
      </StateDeclaration>
      <StateDeclaration LineNo="10" ColNo="8">
         <Name>bump_sensor</Name>
         <Return>
            <Name>_return_0</Name>
            <Type>Boolean</Type>
         </Return>
      </StateDeclaration>
      <StateDeclaration LineNo="11" ColNo="8">
         <Name>current_waypoint</Name>
         <Return>
            <Name>_return_0</Name>
            <Type>Integer</Type>
         </Return>
      </StateDeclaration>
   </GlobalDeclarations>
   <Node NodeType="NodeList" epx="Concurrence" LineNo="13" ColNo="6">
      <NodeId>Main</NodeId>
      <VariableDeclarations>
         <DeclareVariable LineNo="14" ColNo="2">
            <Name>drive_done</Name>
            <Type>Boolean</Type>
            <InitialValue>
               <BooleanValue>false</BooleanValue>
            </InitialValue>
         </DeclareVariable>
         <DeclareVariable LineNo="15" ColNo="2">
            <Name>num_waypoints</Name>
            <Type>Integer</Type>
            <InitialValue>
               <IntegerValue>3</IntegerValue>
            </InitialValue>
         </DeclareVariable>
      </VariableDeclarations>
      <EndCondition>
         <EQBoolean>
            <BooleanVariable>drive_done</BooleanVariable>
            <BooleanValue>true</BooleanValue>
         </EQBoolean>
      </EndCondition>
      <NodeBody>
         <NodeList>
            <Node NodeType="Command" LineNo="24" ColNo="4">
               <NodeId>DriveToWaypoint</NodeId>
               <RepeatCondition>
                  <LT>
                     <LookupOnChange>
                        <Name>
                           <StringValue>current_waypoint</StringValue>
                        </Name>
                     </LookupOnChange>
                     <IntegerVariable>num_waypoints</IntegerVariable>
                  </LT>
               </RepeatCondition>
               <InvariantCondition>
                  <EQBoolean>
                     <LookupNow>
                        <Name>
                           <StringValue>tilt_sensor</StringValue>
                        </Name>
                     </LookupNow>
                     <BooleanValue>false</BooleanValue>
                  </EQBoolean>
               </InvariantCondition>
               <EndCondition>
                  <EQBoolean>
                     <LookupOnChange>
                        <Name>
                           <StringValue>at_waypoint</StringValue>
                        </Name>
                     </LookupOnChange>
                     <BooleanValue>true</BooleanValue>
                  </EQBoolean>
               </EndCondition>
               <NodeBody>
                  <Command>
                     <Name>
                        <StringValue>driveToNextWaypoint</StringValue>
                     </Name>
                  </Command>
               </NodeBody>
            </Node>
            <Node NodeType="Command" LineNo="31" ColNo="4">
               <NodeId>TiltSensor</NodeId>
               <RepeatCondition>
                  <EQBoolean>
                     <BooleanVariable>drive_done</BooleanVariable>
                     <BooleanValue>false</BooleanValue>
                  </EQBoolean>
               </RepeatCondition>
               <StartCondition>
                  <EQBoolean>
                     <LookupOnChange>
                        <Name>
                           <StringValue>tilt_sensor</StringValue>
                        </Name>
                     </LookupOnChange>
                     <BooleanValue>true</BooleanValue>
                  </EQBoolean>
               </StartCondition>
               <NodeBody>
                  <Command>
                     <Name>
                        <StringValue>reverseAndTurn</StringValue>
                     </Name>
                  </Command>
               </NodeBody>
            </Node>
            <Node NodeType="Command" LineNo="38" ColNo="4">
               <NodeId>BumpSensor</NodeId>
               <RepeatCondition>
                  <EQBoolean>
                     <BooleanVariable>drive_done</BooleanVariable>
                     <BooleanValue>false</BooleanValue>
                  </EQBoolean>
               </RepeatCondition>
               <StartCondition>
                  <EQBoolean>
                     <LookupOnChange>
                        <Name>
                           <StringValue>bump_sensor</StringValue>
                        </Name>
                     </LookupOnChange>
                     <BooleanValue>true</BooleanValue>
                  </EQBoolean>
               </StartCondition>
               <NodeBody>
                  <Command>
                     <Name>
                        <StringValue>reverseAndTurn</StringValue>
                     </Name>
                  </Command>
               </NodeBody>
            </Node>
            <Node NodeType="NodeList" epx="Sequence" LineNo="44" ColNo="4">
               <NodeId>DriveToDock</NodeId>
               <StartCondition>
                  <GE>
                     <LookupOnChange>
                        <Name>
                           <StringValue>current_waypoint</StringValue>
                        </Name>
                     </LookupOnChange>
                     <IntegerVariable>num_waypoints</IntegerVariable>
                  </GE>
               </StartCondition>
               <InvariantCondition>
                  <NOT>
                     <OR>
                        <AND>
                           <EQInternal>
                              <NodeOutcomeVariable>
                                 <NodeRef dir="child">StartDock</NodeRef>
                              </NodeOutcomeVariable>
                              <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                           </EQInternal>
                           <EQInternal>
                              <NodeStateVariable>
                                 <NodeRef dir="child">StartDock</NodeRef>
                              </NodeStateVariable>
                              <NodeStateValue>FINISHED</NodeStateValue>
                           </EQInternal>
                        </AND>
                        <AND>
                           <EQInternal>
                              <NodeOutcomeVariable>
                                 <NodeRef dir="child">COMMAND__4</NodeRef>
                              </NodeOutcomeVariable>
                              <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                           </EQInternal>
                           <EQInternal>
                              <NodeStateVariable>
                                 <NodeRef dir="child">COMMAND__4</NodeRef>
                              </NodeStateVariable>
                              <NodeStateValue>FINISHED</NodeStateValue>
                           </EQInternal>
                        </AND>
                        <AND>
                           <EQInternal>
                              <NodeOutcomeVariable>
                                 <NodeRef dir="child">ASSIGNMENT__5</NodeRef>
                              </NodeOutcomeVariable>
                              <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                           </EQInternal>
                           <EQInternal>
                              <NodeStateVariable>
                                 <NodeRef dir="child">ASSIGNMENT__5</NodeRef>
                              </NodeStateVariable>
                              <NodeStateValue>FINISHED</NodeStateValue>
                           </EQInternal>
                        </AND>
                     </OR>
                  </NOT>
               </InvariantCondition>
               <NodeBody>
                  <NodeList>
                     <Node NodeType="Command" LineNo="47" ColNo="6">
                        <NodeId>StartDock</NodeId>
                        <EndCondition>
                           <EQInternal>
                              <NodeCommandHandleVariable>
                                 <NodeId>StartDock</NodeId>
                              </NodeCommandHandleVariable>
                              <NodeCommandHandleValue>COMMAND_SUCCESS</NodeCommandHandleValue>
                           </EQInternal>
                        </EndCondition>
                        <NodeBody>
                           <Command>
                              <Name>
                                 <StringValue>dock</StringValue>
                              </Name>
                           </Command>
                        </NodeBody>
                     </Node>
                     <Node NodeType="Command" LineNo="49" ColNo="4">
                        <NodeId>COMMAND__4</NodeId>
                        <StartCondition>
                           <EQInternal>
                              <NodeStateVariable>
                                 <NodeRef dir="sibling">StartDock</NodeRef>
                              </NodeStateVariable>
                              <NodeStateValue>FINISHED</NodeStateValue>
                           </EQInternal>
                        </StartCondition>
                        <NodeBody>
                           <Command>
                              <Name>
                                 <StringValue>debugMsg</StringValue>
                              </Name>
                              <Arguments LineNo="50" ColNo="14">
                                 <StringValue>MISSION COMPLETE</StringValue>
                              </Arguments>
                           </Command>
                        </NodeBody>
                     </Node>
                     <Node NodeType="Assignment" LineNo="51" ColNo="4">
                        <NodeId>ASSIGNMENT__5</NodeId>
                        <StartCondition>
                           <EQInternal>
                              <NodeStateVariable>
                                 <NodeRef dir="sibling">COMMAND__4</NodeRef>
                              </NodeStateVariable>
                              <NodeStateValue>FINISHED</NodeStateValue>
                           </EQInternal>
                        </StartCondition>
                        <NodeBody>
                           <Assignment>
                              <BooleanVariable>drive_done</BooleanVariable>
                              <BooleanRHS>
                                 <BooleanValue>true</BooleanValue>
                              </BooleanRHS>
                           </Assignment>
                        </NodeBody>
                     </Node>
                  </NodeList>
               </NodeBody>
            </Node>
            <Node NodeType="Command" LineNo="59" ColNo="4">
               <NodeId>UpdateLoop</NodeId>
               <RepeatCondition>
                  <EQBoolean>
                     <BooleanVariable>drive_done</BooleanVariable>
                     <BooleanValue>false</BooleanValue>
                  </EQBoolean>
               </RepeatCondition>
               <NodeBody>
                  <Command>
                     <Name>
                        <StringValue>update</StringValue>
                     </Name>
                  </Command>
               </NodeBody>
            </Node>
         </NodeList>
      </NodeBody>
   </Node>
</PlexilPlan>