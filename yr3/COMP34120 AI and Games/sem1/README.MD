## COMP34120.1 Kalah Agent
A copy from the [most advanced branch](https://github.com/cbwchuck/COMP34120-semester-1-project/tree/With_heuristic), the agent was the first runner-up in the class tournament.


Collaborator: [Bowen Cai](https://github.com/cbwchuck), [Yueer Mao](https://github.com/EjoMYE)

### 0. Background
Build agent for Kalah with minimax & alpha-beta pruning, see game rule [here](docs/Kalah_rules.png)


### 1. Code introduction

#### py
- agent_v1.py (agent entrance): search depth = 1
- agent_v2.py (agent entrance): minimax with alpha-beta pruning, search depth = 3
- agent_v3.py (agent entrance): introduce move ranking & move heuristic for end node eval
- agent_v4.py (agent entrance): maintain global state tree
- Global_state.py: Define game state
- Client.py: I/O to game engine
- Utils.py: Logic & func for debugging and parsing
- autoplay.py: Test the agent's performance
- log.txt: Record Debug information


### 2. Getting Started
#### Play manually
start a terminal,
```
nc localhost 12345
```
then start another one,
```
java -jar ManKalah.jar "python3 py/agent_v3.py" "nc localhost 12345"
```
now the game starts, go to the 1st terminal you opened to make your move.

#### Python
install packages
```
python -m venv kalah
source kalah/bin/activate
pip install -r requirements.txt
```
then run game with python agent following example in run-commands.txt.

Every time you open a new terminal, remember to activate venv:
```
source kalah/bin/activate
```
before running a python agent.

#### Java
**Use jdk 1.8 or below**, as some libs became depricated since 1.9

Open 'java' folder in IntelleJ, follow [this guide](https://www.jetbrains.com/help/idea/compiling-applications.html#package_into_jar) to build sem1.jar, then start the game following examples in run-commands.txt to run the jar as a player.

### 3. Technologies:
Java, python