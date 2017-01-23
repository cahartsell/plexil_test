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
   </GlobalDeclarations>
   <Node NodeType="NodeList" epx="Concurrence" LineNo="4" ColNo="6">
      <NodeId>Main</NodeId>
      <VariableDeclarations>
         <DeclareVariable LineNo="5" ColNo="2">
            <Name>bar</Name>
            <Type>Integer</Type>
            <InitialValue>
               <IntegerValue>0</IntegerValue>
            </InitialValue>
         </DeclareVariable>
         <DeclareArray LineNo="5" ColNo="2">
            <Name>foo</Name>
            <Type>Integer</Type>
            <MaxSize>4</MaxSize>
            <InitialValue>
               <IntegerValue>0</IntegerValue>
               <IntegerValue>0</IntegerValue>
               <IntegerValue>0</IntegerValue>
               <IntegerValue>0</IntegerValue>
            </InitialValue>
         </DeclareArray>
         <DeclareVariable LineNo="6" ColNo="2">
            <Name>temp</Name>
            <Type>Real</Type>
            <InitialValue>
               <RealValue>0.0</RealValue>
            </InitialValue>
         </DeclareVariable>
      </VariableDeclarations>
      <NodeBody>
         <NodeList>
            <Node NodeType="NodeList" epx="While" LineNo="8" ColNo="2">
               <NodeId>while__0</NodeId>
               <RepeatCondition>
                  <EQInternal>
                     <NodeOutcomeVariable>
                        <NodeRef dir="child">ep2cp_WhileTest</NodeRef>
                     </NodeOutcomeVariable>
                     <NodeOutcomeValue>SUCCESS</NodeOutcomeValue>
                  </EQInternal>
               </RepeatCondition>
               <NodeBody>
                  <NodeList>
                     <Node NodeType="Empty" epx="Condition">
                        <NodeId>ep2cp_WhileTest</NodeId>
                        <PostCondition>
                           <LT>
                              <RealVariable>temp</RealVariable>
                              <RealValue>3.0</RealValue>
                           </LT>
                        </PostCondition>
                     </Node>
                     <Node NodeType="NodeList" epx="Action" LineNo="10" ColNo="4">
                        <NodeId>BLOCK__1</NodeId>
                        <InvariantCondition>
                           <NOT>
                              <OR>
                                 <AND>
                                    <EQInternal>
                                       <NodeOutcomeVariable>
                                          <NodeRef dir="child">IncreaseTemp</NodeRef>
                                       </NodeOutcomeVariable>
                                       <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                                    </EQInternal>
                                    <EQInternal>
                                       <NodeStateVariable>
                                          <NodeRef dir="child">IncreaseTemp</NodeRef>
                                       </NodeStateVariable>
                                       <NodeStateValue>FINISHED</NodeStateValue>
                                    </EQInternal>
                                 </AND>
                                 <AND>
                                    <EQInternal>
                                       <NodeOutcomeVariable>
                                          <NodeRef dir="child">ArrayAssignment</NodeRef>
                                       </NodeOutcomeVariable>
                                       <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                                    </EQInternal>
                                    <EQInternal>
                                       <NodeStateVariable>
                                          <NodeRef dir="child">ArrayAssignment</NodeRef>
                                       </NodeStateVariable>
                                       <NodeStateValue>FINISHED</NodeStateValue>
                                    </EQInternal>
                                 </AND>
                              </OR>
                           </NOT>
                        </InvariantCondition>
                        <StartCondition>
                           <EQInternal>
                              <NodeOutcomeVariable>
                                 <NodeRef dir="sibling">ep2cp_WhileTest</NodeRef>
                              </NodeOutcomeVariable>
                              <NodeOutcomeValue>SUCCESS</NodeOutcomeValue>
                           </EQInternal>
                        </StartCondition>
                        <SkipCondition>
                           <AND>
                              <EQInternal>
                                 <NodeStateVariable>
                                    <NodeRef dir="sibling">ep2cp_WhileTest</NodeRef>
                                 </NodeStateVariable>
                                 <NodeStateValue>FINISHED</NodeStateValue>
                              </EQInternal>
                              <EQInternal>
                                 <NodeFailureVariable>
                                    <NodeRef dir="sibling">ep2cp_WhileTest</NodeRef>
                                 </NodeFailureVariable>
                                 <NodeFailureValue>POST_CONDITION_FAILED</NodeFailureValue>
                              </EQInternal>
                           </AND>
                        </SkipCondition>
                        <NodeBody>
                           <NodeList>
                              <Node NodeType="Assignment" LineNo="10" ColNo="20">
                                 <NodeId>IncreaseTemp</NodeId>
                                 <NodeBody>
                                    <Assignment>
                                       <RealVariable>temp</RealVariable>
                                       <NumericRHS>
                                          <ADD LineNo="10" ColNo="32">
                                             <RealVariable>temp</RealVariable>
                                             <IntegerValue>1</IntegerValue>
                                          </ADD>
                                       </NumericRHS>
                                    </Assignment>
                                 </NodeBody>
                              </Node>
                              <Node NodeType="NodeList" epx="Sequence" LineNo="13" ColNo="6">
                                 <NodeId>ArrayAssignment</NodeId>
                                 <StartCondition>
                                    <EQInternal>
                                       <NodeStateVariable>
                                          <NodeRef dir="sibling">IncreaseTemp</NodeRef>
                                       </NodeStateVariable>
                                       <NodeStateValue>FINISHED</NodeStateValue>
                                    </EQInternal>
                                 </StartCondition>
                                 <InvariantCondition>
                                    <NOT>
                                       <OR>
                                          <AND>
                                             <EQInternal>
                                                <NodeOutcomeVariable>
                                                   <NodeRef dir="child">ASSIGNMENT__3</NodeRef>
                                                </NodeOutcomeVariable>
                                                <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                                             </EQInternal>
                                             <EQInternal>
                                                <NodeStateVariable>
                                                   <NodeRef dir="child">ASSIGNMENT__3</NodeRef>
                                                </NodeStateVariable>
                                                <NodeStateValue>FINISHED</NodeStateValue>
                                             </EQInternal>
                                          </AND>
                                          <AND>
                                             <EQInternal>
                                                <NodeOutcomeVariable>
                                                   <NodeRef dir="child">ASSIGNMENT__4</NodeRef>
                                                </NodeOutcomeVariable>
                                                <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                                             </EQInternal>
                                             <EQInternal>
                                                <NodeStateVariable>
                                                   <NodeRef dir="child">ASSIGNMENT__4</NodeRef>
                                                </NodeStateVariable>
                                                <NodeStateValue>FINISHED</NodeStateValue>
                                             </EQInternal>
                                          </AND>
                                          <AND>
                                             <EQInternal>
                                                <NodeOutcomeVariable>
                                                   <NodeRef dir="child">COMMAND__5</NodeRef>
                                                </NodeOutcomeVariable>
                                                <NodeOutcomeValue>FAILURE</NodeOutcomeValue>
                                             </EQInternal>
                                             <EQInternal>
                                                <NodeStateVariable>
                                                   <NodeRef dir="child">COMMAND__5</NodeRef>
                                                </NodeStateVariable>
                                                <NodeStateValue>FINISHED</NodeStateValue>
                                             </EQInternal>
                                          </AND>
                                       </OR>
                                    </NOT>
                                 </InvariantCondition>
                                 <NodeBody>
                                    <NodeList>
                                       <Node NodeType="Assignment" LineNo="13" ColNo="6">
                                          <NodeId>ASSIGNMENT__3</NodeId>
                                          <NodeBody>
                                             <Assignment>
                                                <ArrayElement>
                                                   <Name>foo</Name>
                                                   <Index>
                                                      <IntegerValue>1</IntegerValue>
                                                   </Index>
                                                </ArrayElement>
                                                <NumericRHS>
                                                   <ADD LineNo="13" ColNo="22">
                                                      <ArrayElement>
                                                         <Name>foo</Name>
                                                         <Index>
                                                            <IntegerValue>1</IntegerValue>
                                                         </Index>
                                                      </ArrayElement>
                                                      <IntegerValue>1</IntegerValue>
                                                   </ADD>
                                                </NumericRHS>
                                             </Assignment>
                                          </NodeBody>
                                       </Node>
                                       <Node NodeType="Assignment" LineNo="14" ColNo="6">
                                          <NodeId>ASSIGNMENT__4</NodeId>
                                          <StartCondition>
                                             <EQInternal>
                                                <NodeStateVariable>
                                                   <NodeRef dir="sibling">ASSIGNMENT__3</NodeRef>
                                                </NodeStateVariable>
                                                <NodeStateValue>FINISHED</NodeStateValue>
                                             </EQInternal>
                                          </StartCondition>
                                          <NodeBody>
                                             <Assignment>
                                                <ArrayElement>
                                                   <Name>foo</Name>
                                                   <Index>
                                                      <IntegerValue>2</IntegerValue>
                                                   </Index>
                                                </ArrayElement>
                                                <NumericRHS>
                                                   <ADD LineNo="14" ColNo="22">
                                                      <ArrayElement>
                                                         <Name>foo</Name>
                                                         <Index>
                                                            <IntegerValue>2</IntegerValue>
                                                         </Index>
                                                      </ArrayElement>
                                                      <IntegerValue>2</IntegerValue>
                                                   </ADD>
                                                </NumericRHS>
                                             </Assignment>
                                          </NodeBody>
                                       </Node>
                                       <Node NodeType="Command" LineNo="14" ColNo="6">
                                          <NodeId>COMMAND__5</NodeId>
                                          <StartCondition>
                                             <EQInternal>
                                                <NodeStateVariable>
                                                   <NodeRef dir="sibling">ASSIGNMENT__4</NodeRef>
                                                </NodeStateVariable>
                                                <NodeStateValue>FINISHED</NodeStateValue>
                                             </EQInternal>
                                          </StartCondition>
                                          <NodeBody>
                                             <Command>
                                                <Name>
                                                   <StringValue>debugMsg</StringValue>
                                                </Name>
                                                <Arguments LineNo="15" ColNo="15">
                                                   <StringValue>test</StringValue>
                                                </Arguments>
                                             </Command>
                                          </NodeBody>
                                       </Node>
                                    </NodeList>
                                 </NodeBody>
                              </Node>
                           </NodeList>
                        </NodeBody>
                     </Node>
                  </NodeList>
               </NodeBody>
            </Node>
            <Node NodeType="NodeList" epx="While" LineNo="19" ColNo="2">
               <NodeId>while__6</NodeId>
               <RepeatCondition>
                  <EQInternal>
                     <NodeOutcomeVariable>
                        <NodeRef dir="child">ep2cp_WhileTest</NodeRef>
                     </NodeOutcomeVariable>
                     <NodeOutcomeValue>SUCCESS</NodeOutcomeValue>
                  </EQInternal>
               </RepeatCondition>
               <NodeBody>
                  <NodeList>
                     <Node NodeType="Empty" epx="Condition">
                        <NodeId>ep2cp_WhileTest</NodeId>
                        <PostCondition>
                           <GT>
                              <RealVariable>temp</RealVariable>
                              <RealValue>-5.0</RealValue>
                           </GT>
                        </PostCondition>
                     </Node>
                     <Node NodeType="Assignment" epx="Action" LineNo="21" ColNo="4">
                        <NodeId>ASSIGNMENT__8</NodeId>
                        <StartCondition>
                           <EQInternal>
                              <NodeOutcomeVariable>
                                 <NodeRef dir="sibling">ep2cp_WhileTest</NodeRef>
                              </NodeOutcomeVariable>
                              <NodeOutcomeValue>SUCCESS</NodeOutcomeValue>
                           </EQInternal>
                        </StartCondition>
                        <SkipCondition>
                           <AND>
                              <EQInternal>
                                 <NodeStateVariable>
                                    <NodeRef dir="sibling">ep2cp_WhileTest</NodeRef>
                                 </NodeStateVariable>
                                 <NodeStateValue>FINISHED</NodeStateValue>
                              </EQInternal>
                              <EQInternal>
                                 <NodeFailureVariable>
                                    <NodeRef dir="sibling">ep2cp_WhileTest</NodeRef>
                                 </NodeFailureVariable>
                                 <NodeFailureValue>POST_CONDITION_FAILED</NodeFailureValue>
                              </EQInternal>
                           </AND>
                        </SkipCondition>
                        <NodeBody>
                           <Assignment>
                              <IntegerVariable>bar</IntegerVariable>
                              <NumericRHS>
                                 <SUB LineNo="21" ColNo="14">
                                    <IntegerVariable>bar</IntegerVariable>
                                    <IntegerValue>1</IntegerValue>
                                 </SUB>
                              </NumericRHS>
                           </Assignment>
                        </NodeBody>
                     </Node>
                  </NodeList>
               </NodeBody>
            </Node>
         </NodeList>
      </NodeBody>
   </Node>
</PlexilPlan>