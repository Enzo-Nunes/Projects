from pulp import *

N, P, Max = map(int, input().split())
toys = []
toy_vars = []
for _ in range(N):
	toy_profit, toy_max_prod = map(int, input().split())
	toys.append((toy_profit, toy_max_prod, []))
	toy_vars.append(LpVariable("toy_" + str(_), lowBound=0, upBound=toy_max_prod, cat=const.LpInteger))
packs = []
pack_vars = []
for _ in range(P):
	toy1, toy2, toy3, pack_profit = map(int, input().split())
	toys[toy1-1][2].append(_)
	toys[toy2-1][2].append(_)
	toys[toy3-1][2].append(_)
	packs.append(pack_profit)
	pack_vars.append(LpVariable("pack_" + str(_), lowBound=0, cat=const.LpInteger))

natal = LpProblem("Natal", LpMaximize)

for i in range(N):
	natal += toy_vars[i] + lpSum(pack_vars[j] for j in toys[i][2]) <= toys[i][1]

natal += lpSum(toy_vars[i]*toys[i][0] for i in range(N)) + lpSum(pack_vars[i]*packs[i] for i in range(P))

natal += lpSum(toy_vars[i] for i in range(N)) + lpSum(pack_vars[i]*3 for i in range(P)) <= Max

natal.solve(GLPK(msg=0))
print(int(value(natal.objective)))