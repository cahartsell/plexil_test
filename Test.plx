<?xml version="1.0" encoding="UTF-8"?>
<PlexilPlan xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:tr="extended-plexil-translator"
            FileName="/home/charles/plexil-4.0.1/examples/basic/plexil_test/Test.ple">
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
         <Name>update</Name>
      </CommandDeclaration>
      <StateDeclaration LineNo="6" ColNo="8">
         <Name>at_waypoint</Name>
         <Return>
            <Name>_return_0</Name>
            <Type>Boolean</Type>
         </Return>
      </StateDeclaration>
   </GlobalDeclarations>
   <Node NodeType="NodeList" epx="Concurrence" LineNo="8" ColNo="6">
      <NodeId>Main</NodeId>
      <VariableDeclarations>
         <DeclareVariable LineNo="9" ColNo="2">
            <Name>drive_done</Name>
            <Type>Boolean</Type>
            <InitialValue>
               <BooleanValue>false</BooleanValue>
            </InitialValue>
         </DeclareVariable>
      </VariableDeclarations>
      <NodeBody>
         <NodeList>
            <Node NodeType="Command" LineNo="15" ColNo="4">
               <NodeId>DriveToWaypoint</NodeId>
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
                           <StringValue>at_waypoint</StringValue>
                        </Name>
                     </LookupOnChange>
                     <BooleanValue>true</BooleanValue>
                  </EQBoolean>
               </StartCondition>
               <NodeBody>
                  <Command>
                     <Name>
                        <StringValue>driveToNextWaypoint</StringValue>
                     </Name>
                  </Command>
               </NodeBody>
            </Node>
            <Node NodeType="Command" LineNo="21" ColNo="4">
               <NodeId>UpdateLoop</NodeId>
               <RepeatCondition>
                  <BooleanValue>true</BooleanValue>
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