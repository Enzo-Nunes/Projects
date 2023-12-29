from pulp import *

N, P, Max = map(int, input().split())

# Initialize empty lists to store information about toys and packs
toys = []
toy_vars = []
packs = []
pack_vars = []

# Read information about each toy from the input
for _ in range(N):
	toy_profit, toy_max_prod = map(int, input().split())
	toys.append((toy_profit, toy_max_prod, []))
	toy_vars.append(LpVariable("toy_" + str(_), lowBound=0, upBound=toy_max_prod, cat=const.LpInteger))

# Read information about each pack from the input
for _ in range(P):
	toy1, toy2, toy3, pack_profit = map(int, input().split())
	packs.append(pack_profit)
	toys[toy1-1][2].append(_)
	toys[toy2-1][2].append(_)
	toys[toy3-1][2].append(_)
	pack_vars.append(LpVariable("pack_" + str(_), lowBound=0, cat=const.LpInteger))

# Create a new LP maximization problem
natal = LpProblem("Natal", LpMaximize)

# Add constraints to the LP problem for each toy
for i in range(N):
	# Sum of the toy's production and the production of packs it belongs to should be less than or equal to its maximum production
	natal += toy_vars[i] + lpSum(pack_vars[j] for j in toys[i][2]) <= toys[i][1]

# Set the objective function of the LP problem
natal += lpSum(toy_vars[i]*toys[i][0] for i in range(N)) + lpSum(pack_vars[i]*packs[i] for i in range(P))

# Add a constraint to the LP problem that limits the total production and packs used
natal += lpSum(toy_vars[i] for i in range(N)) + lpSum(pack_vars[i]*3 for i in range(P)) <= Max

# Solve the LP problem using the GLPK solver with the message level set to 0 (no output)
natal.solve(GLPK(msg=0))

# Print the status of the solution
print("Status:", LpStatus[natal.status])

# Print the value of the objective function
print("Maximum Profit:", value(natal.objective))

# Print the values of the decision variables
for i in range(N):
    print(f'Toy {i+1}: {value(toy_vars[i])}')
for i in range(P):
    print(f'Pack {i+1}: {value(pack_vars[i])}')

# Print the total number of toys
total_toys = sum(value(toy_vars[i]) for i in range(N)) + sum(value(pack_vars[i])*3 for i in range(P))
print("Total number of toys:", total_toys)