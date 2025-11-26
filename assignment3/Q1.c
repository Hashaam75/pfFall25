#include <stdio.h>

double calculateRepayment(double loan, double interestRate, int years, double installment) {
    
    
    if (loan <= 0 || years == 0) {
        printf("Loan fully repaid.\n");
        return 0;
    }

    loan = loan + (loan * interestRate);

    double remaining = loan - installment;
    if (remaining < 0) remaining = 0;

    printf("Year %d: Remaining loan = %.2f\n", years, remaining);

    return installment + calculateRepayment(remaining, interestRate, years - 1, installment);
}

int main() {
    double loan = 100000; 
    double interestRate = 0.05;
    int years = 3;
    double installment = 40000;  

    double totalPaid = calculateRepayment(loan, interestRate, years, installment);

    printf("\nTotal amount paid over %d years = %.2f\n", years, totalPaid);

    return 0;
}

\\extended version
double calculateRepaymentExtra(double loan, double interestRate, int years, 
                               double installment, double extra) {

    if (loan <= 0 || years == 0) {
        printf("Loan fully repaid.\n");
        return 0;
    }

    loan = loan + loan * interestRate;
    loan = loan - (installment + extra);
    if (loan < 0) loan = 0;

    printf("Year %d: Remaining loan = %.2f\n", years, loan);

    return installment + extra + 
           calculateRepaymentExtra(loan, interestRate, years - 1, installment, extra);
}
\\Base case
if (loan <= 0 || years == 0)
    return 0;
\\Recursion is useful because:
\\Each year’s loan depends on previous year's remaining loan
\\The state naturally progresses step-by-step

