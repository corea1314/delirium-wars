<ASM>
	<Parameters>
		<Parameter>
			<Type>float</Type>
			<Name>Speed</Name>			
		</Parameter>
		<Parameter>
			<Type>bool</Type>
			<Name>Jump</Name>			
		</Parameter>
		<Parameter>
			<Type>bool</Type>
			<Name>Throw</Name>			
		</Parameter>
	</Parameters>
	<Layers>
		<Layer>
			<Name>Base</Name>
			<InitialStateName>Idle</InitialStateName>
			<States>
				<State>
					<Name>Idle</Name>
					<SequenceName>pilot/idle.spr</SequenceName>
					<Loop>true</Loop>
					<Transitions>
						<Transition>
							<TransitStateName>Walking</TransitStateName>
							<Conditions>
								<ConditionParameter>
									<Parameter>Speed</Parameter>
									<Operator>Greater</Operator>
									<Value>0.0</Value>				
								</ConditionParameter>
							</Conditions>
						</Transition>
						<Transition>
							<TransitStateName>Jumping</TransitStateName>
							<Conditions>
								<ConditionParameter>
									<Parameter>Jump</Parameter>
									<Operator>Equal</Operator>
									<Value>true</Value>				
								</ConditionParameter>
							</Conditions>
						</Transition>
						<Transition>
							<TransitStateName>Throwing</TransitStateName>
							<Conditions>
								<ConditionParameter>
									<Parameter>Throw</Parameter>
									<Operator>Equal</Operator>
									<Value>true</Value>				
								</ConditionParameter>
							</Conditions>
						</Transition>
					</Transitions>
				</State>
				<State>
					<Name>Walking</Name>
          <Loop>true</Loop>
					<SequenceName>pilot/walking.spr</SequenceName>
					<Transitions>
						<Transition>
							<TransitStateName>Idle</TransitStateName>
							<Conditions>
								<ConditionParameter>
									<Parameter>Speed</Parameter>
									<Operator>Less</Operator>
									<Value>0.1</Value>				
								</ConditionParameter>
							</Conditions>
						</Transition>
						<Transition>
							<TransitStateName>Jumping</TransitStateName>
							<Conditions>
								<ConditionParameter>
									<Parameter>Jump</Parameter>
									<Operator>Equal</Operator>
									<Value>true</Value>				
								</ConditionParameter>
							</Conditions>
						</Transition>
					</Transitions>
				</State>
				<State>
					<Name>Throwing</Name>
					<SequenceName>pilot/throwing.spr</SequenceName>
					<Transitions>
						<Transition>
							<TransitStateName>Idle</TransitStateName>
							<Conditions>
								<ConditionSequenceEnd>
								</ConditionSequenceEnd>
							</Conditions>
						</Transition>
					</Transitions>
				</State>
				<State>
					<Name>Jumping</Name>
					<SequenceName>pilot/jumping.spr</SequenceName>
					<Transitions>
						<Transition>
							<TransitStateName>Idle</TransitStateName>
							<Conditions>
								<ConditionParameter>
									<Parameter>Speed</Parameter>
									<Operator>Less</Operator>
									<Value>0.1</Value>				
								</ConditionParameter>
								<ConditionSequenceEnd>
								</ConditionSequenceEnd>
							</Conditions>
						</Transition>
						<Transition>
							<TransitStateName>Walking</TransitStateName>
							<Conditions>
								<ConditionParameter>
									<Parameter>Speed</Parameter>
									<Operator>Greater</Operator>
									<Value>1.0</Value>				
								</ConditionParameter>
								<ConditionSequenceEnd>
								</ConditionSequenceEnd>
							</Conditions>
						</Transition>
					</Transitions>
				</State>
			</States>
		</Layer>
	</Layers>
</ASM>