def main():
    num_variables = int(input())
    num_clauses = int(input())
    clauses = []

    for _ in range(num_clauses):
        a, b, c = input().split()
        clauses.append([int(a), int(b), int(c)])

    assigned_variables = input().split()
    print(len(assigned_variables))

    for i in range(num_variables):
        assigned_variables[i] = int(assigned_variables[i])

    for clause in clauses:
        is_clause_satisfied = False
        for literal in clause:
            if literal > 0 and assigned_variables[literal - 1] == 1:
                is_clause_satisfied = True
                break
            if literal < 0 and assigned_variables[-literal - 1] == 0:
                is_clause_satisfied = True
                break

        if not is_clause_satisfied:
            print("这个解是错误的")
            return

    print("这是一个可行解")


if __name__ == '__main__':
    main()
