X (is simple minimax) and O (is minimax with alpha beta pruning)
Both are playing against each other (AI VS AI)

* Compare the performance of the standard Minimax and the Alpha-Beta Pruning optimized Minimax.
1. Speed: In the context of Tic-Tac-Toe, Alpha-Beta Pruning significantly speeds up the AI's decision-making by pruning unnecessary branches, allowing the AI to make moves faster than the standard Minimax.

2. Efficiency: Alpha-Beta Pruning evaluates fewer nodes in the game tree, resulting in a more efficient search process. This means the AI can analyze deeper game states in less time compared to Minimax.

3. Time Complexity: Both Minimax and Alpha-Beta Pruning have similar time complexities in the worst case, but Alpha-Beta pruning can cut the number of nodes explored in half, making it more optimal in practice for games like Tic-Tac-Toe.

4. Game Play: Alpha-Beta Pruning leads to quicker gameplay in Tic-Tac-Toe, providing faster AI responses, while the standard Minimax results in slower moves due to the evaluation of more branches in the game tree.
