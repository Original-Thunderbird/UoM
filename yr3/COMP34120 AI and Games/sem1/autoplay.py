import subprocess
import os
import time

play_times = 1 ## set the times of playing the game
north_win = 0
south_win = 0

''' comment out to abandon the log '''
# player1_commands = ['python3 0124.py', 'python3 012.py', 'python3 0.py']
# player1_commands = ['python3 0124.py']
player1_commands = ['python3 py/mainMinimax_h.py']
player2_commands = ['java -jar opponent/error404.jar', 'java -jar opponent/JimmyPlayer.jar', 'java -jar opponent/Group2Agent.jar']

for player1_command in player1_commands:
    for player2_command in player2_commands:
        print(f"\n\n\nCommand - {player1_command}, {player2_command}\n")
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
        print("\n\***************** NOW reverse player positions ***************** \n")
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
