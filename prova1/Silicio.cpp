#include <iostream>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TLegend.h>
#include <cmath>
#include <TROOT.h>

int Silicio()
{
    gROOT->SetBatch(kFALSE); // Forza ROOT a mostrare la finestra grafica

    // Define the data points
    const int nPoints = 21;
    double V[nPoints] = {700, 680, 660, 640, 620, 610, 600, 590, 580, 570, 560, 550, 540, 530, 520, 510, 500, 490, 480, 470, 460};
    double I[nPoints] = {3.65, 2.8, 2.02, 1.33, 0.88, 0.72, 0.58, 0.47, 0.36, 0.31, 0.24, 0.2, 0.16, 0.12, 0.1, 0.09, 0.07, 0.06, 0.05, 0.04, 0.03};
    double V_err[nPoints] = {23.3, 22.7, 22.2, 21.6, 21.1, 20.9, 20.6, 20.3, 20.1, 19.8, 19.6, 19.3, 19.0, 18.8, 18.5, 18.3, 18.0, 17.8, 17.5, 17.3, 17.0};
    double I_err[nPoints] = {0.0748, 0.0620, 0.0503, 0.0400, 0.0332, 0.0308, 0.0287, 0.0271, 0.0254, 0.0247, 0.0236, 0.0230, 0.0224, 0.0218, 0.0215, 0.0214, 0.0211, 0.0209, 0.0208, 0.0206, 0.0205};

    // Create the TGraphErrors for the original data
    TGraphErrors *graph = new TGraphErrors(nPoints, V, I, V_err, I_err);

    // Set graph styles
    graph->SetTitle("I vs V;V (mV);I (mA)");
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(1.0);

    // Create a canvas for the graphs
    TCanvas *c1 = new TCanvas("c1", "Silicio and Fit", 1200, 600);
    c1->Divide(2, 1); // Divide canvas into two pads

    // Pad 1: Linear scale graph
    c1->cd(1);
    c1->cd(1)->SetGrid();
    graph->GetXaxis()->SetLimits(400, 800);
    graph->GetYaxis()->SetRangeUser(0, 4);
    graph->Draw("AP");

    // Fit function: I = I0 * (exp(V / (eta * VT)) - 1)
    TF1 *fitFunc = new TF1("fitFunc", "[0]*(exp((x-[2])/[1]) - 1)", 450, 750); // Fit range
    fitFunc->SetParameters(0.01, 50, 450);                                     // Initial guesses for I0, eta*VT, V0
    fitFunc->SetParNames("I_0", "eta*V_T", "V_offset");
    graph->Fit(fitFunc, "R"); // Perform the fit with range constraint

    // Pad 2: Logarithmic scale graph
    c1->cd(2);
    c1->cd(2)->SetGrid();
    c1->cd(2)->SetLogy(); // Set logarithmic scale on Y-axis
    TGraphErrors *graphLog = new TGraphErrors(nPoints, V, I, V_err, I_err);
    graphLog->SetTitle("Silicio_log;V (mV);I (mA)");
    graphLog->SetMarkerStyle(21);
    graphLog->SetMarkerSize(1.0);
    graphLog->GetXaxis()->SetLimits(400, 800);
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
