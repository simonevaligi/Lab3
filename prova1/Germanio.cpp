#include <iostream>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TLegend.h>
#include <cmath>
#include <TROOT.h>

int Germanio()
{
    gROOT->SetBatch(kFALSE); // Forza ROOT a mostrare la finestra grafica

    // Define the data points
    const int nPoints = 24;
    double V[nPoints] = {300, 290, 280, 270, 260, 250, 240, 230, 220, 205, 200, 190, 180, 170, 160, 150, 140, 130, 120, 110, 102, 94, 80, 76};
    double I[nPoints] = {2.22, 2.02, 1.62, 1.44, 1.18, 1.01, 0.85, 0.72, 0.61, 0.47, 0.43, 0.35, 0.3, 0.24, 0.2, 0.16, 0.14, 0.11, 0.09, 0.07, 0.06, 0.05, 0.03, 0.03};
    double V_err[nPoints] = {10.3, 10.0, 9.78, 9.52, 9.26, 9.01, 8.77, 8.52, 8.28, 7.93, 7.81, 7.58, 7.36, 7.14, 6.93, 6.73, 4.65, 4.38, 4.12, 3.86, 3.66, 3.46, 3.12, 3.03};
    double I_err[nPoints] = {0.0533, 0.0503, 0.0443, 0.0416, 0.0377, 0.0352, 0.0328, 0.0308, 0.0292, 0.0271, 0.0265, 0.0253, 0.0245, 0.0236, 0.0230, 0.0224, 0.0221, 0.0217, 0.0214, 0.0211, 0.0209, 0.0208, 0.0205, 0.0205};

    // Create the TGraphErrors for the original data
    TGraphErrors *graph = new TGraphErrors(nPoints, V, I, V_err, I_err);

    // Set graph styles
    graph->SetTitle("I vs V;V (mV);I (mA)");
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(1.0);

    // Create a canvas for the graphs
    TCanvas *c1 = new TCanvas("c1", "Germanio and Fit", 1200, 600);
    c1->Divide(2, 1); // Divide canvas into two pads

    // Pad 1: Linear scale graph
    c1->cd(1);
    c1->cd(1)->SetGrid();
    graph->GetXaxis()->SetLimits(0, 400);
    graph->GetYaxis()->SetRangeUser(0, 3);
    graph->Draw("AP");

    // Fit function: I = I0 * (exp((V - V_offset) / (eta * VT)) - 1)
    TF1 *fitFunc = new TF1("fitFunc", "[0]*(exp((x-[2])/[1]) - 1)", 100, 310); // Fit range
    fitFunc->SetParameters(0.01, 50, 90);                                      // Initial guesses for I0, eta*VT, V_offset
    fitFunc->SetParNames("I_0", "eta*V_T", "V_offset");
    graph->Fit(fitFunc, "R"); // Perform the fit with range constraint

    // Pad 2: Logarithmic scale graph
    c1->cd(2);
    c1->cd(2)->SetGrid();
    c1->cd(2)->SetLogy(); // Set logarithmic scale on Y-axis
    TGraphErrors *graphLog = new TGraphErrors(nPoints, V, I, V_err, I_err);
    graphLog->SetTitle("Germanio_log;V (mV);I (mA)");
    graphLog->SetMarkerStyle(21);
    graphLog->SetMarkerSize(1.0);
    graphLog->GetXaxis()->SetLimits(0, 400);
    graphLog->GetYaxis()->SetRangeUser(0.01, 10); // Adjust to avoid negative values in log scale
    graphLog->Draw("AP");
    fitFunc->Draw("same");

    // Add a legend to the log-scale graph
    TLegend *legend = new TLegend(0.1, 0.7, 0.4, 0.9);
    legend->AddEntry(graphLog, "Data points", "p");
    legend->AddEntry(fitFunc, "Fit: I = I_0 * (e^((V-V0)/(eta*V_T)) - 1)", "l");
    legend->Draw();

    // Print fit parameters
    std::cout << "Fit Results:" << std::endl;
    std::cout << "I_0: " << fitFunc->GetParameter(0) << " ± " << fitFunc->GetParError(0) << std::endl;
    std::cout << "eta*V_T: " << fitFunc->GetParameter(1) << " ± " << fitFunc->GetParError(1) << std::endl;
    std::cout << "V_offset: " << fitFunc->GetParameter(2) << " ± " << fitFunc->GetParError(2) << std::endl;

    return 0;
}
