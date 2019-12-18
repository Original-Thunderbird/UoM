import subprocess
import os
import time

play_times = 1 ## set the times of playing the game
north_win = 0
south_win = 0

''' comment out to abandon the log '''
# player1_commands = ['python3 0124.py', 'python3 012.py', 'python3 0.py']
# player1_commands = ['python3 0124.py']
player1_commands = ['java -jar heuristic.jar']
player2_commands = ['java -jar error404.jar', 'java -jar JimmyPlayer.jar', 'java -jar Group2Agent.jar', 'java -jar AbandonedAgents/006AgentGroup6.jar', 'java -jar AbandonedAgents/Group13Bot.jar', 'java -jar AbandonedAgents/Group7Agent.jar', 'java -jar AbandonedAgents/MKAgent23.jar', 'java -jar AbandonedAgents/Agent3.jar', 'java -jar AbandonedAgents/KSAgent2.jar', 'java -jar AbandonedAgents/MKAgent4.jar', 'java -jar AbandonedAgents/BotGroup4.jar', 'java -jar AbandonedAgents/Group27.jar', 'java -jar AbandonedAgents/MANCalaAgent17.jar', 'java -jar AbandonedAgents/group13.jar', 'java -jar AbandonedAgents/Group1.jar', 'java -jar AbandonedAgents/Group27Agent.jar', 'java -jar AbandonedAgents/MKAgent14.jar', 'java -jar baseline.jar']
# player2_commands = ['java -jar move2.jar', 'java -jar move3.jar', 'java -jar move4.jar', 'java -jar move5.jar', 'java -jar move6.jar', 'java -jar move7.jar', 'java -jar baseline.jar']

for player1_command in player1_commands:
    for player2_command in player2_commands:
        print(f"Command - {player1_command}, {player2_command}\n")
        # with open(os.devnull, 'w') as log_file:
        with open('./result.txt', 'w') as log_file:
            start = time.time()
            games_result = dict()
            for i in range(play_times):
                ''' command used to play each game '''
                game = subprocess.run(['java', '-jar', 'ManKalah.jar', player1_command, player2_command], stderr=subprocess.PIPE, stdout=log_file)
                result = game.stderr.decode('utf-8')
                paragraphs = result.split("\n")
                for p in paragraphs:
                    if "WINNER" in p:
                        if p in games_result:
                            games_result[p] += 1
                        else:
                            games_result[p] = 1
                        print(p)
                    if "SCORE" in p:
                        print(f"{p}\n")

        end = time.time()
        print(games_result)
        print(f"Time elapsed - {end-start}s")
        print("\n\n ***************** NOW reverse player positions ***************** \n\n")
        print(f"Command - {player2_command}, {player1_command}\n")

        with open(os.devnull, 'w') as log_file:
        # with open('./result.txt', 'w') as log_file:
            start = time.time()
            games_result = dict()
            for i in range(play_times):
                ''' command used to play each game '''
                game = subprocess.run(['java', '-jar', 'ManKalah.jar', player2_command, player1_command], stderr=subprocess.PIPE, stdout=log_file)
                result = game.stderr.decode('utf-8')
                paragraphs = result.split("\n")
                for p in paragraphs:
                    if "WINNER" in p:
                        if p in games_result:
                            games_result[p] += 1
                        else:
                            games_result[p] = 1
                        print(p)
                    if "SCORE" in p:
                        print(f"{p}\n")

        end = time.time()
        print(games_result)
        print(f"Time elapsed - {end-start}s")
