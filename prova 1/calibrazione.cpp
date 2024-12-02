#include <iostream>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TApplication.h>

int main(int argc, char **argv)
{
    // Initialize the ROOT application (handles the graphical environment)
    TApplication app("app", &argc, argv);

    // Define the data points
    const int nPoints = 12;
    double V_multimeter[nPoints] = {47, 77, 96, 147, 197, 245, 296, 390, 486, 586, 687, 786};
    double V_oscilloscope[nPoints] = {50, 70, 100, 150, 200, 250, 300, 400, 500, 600, 700, 800};
    double V_multimeter_err[nPoints] = {0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1};
    double V_oscilloscope_err[nPoints] = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2};

    // Create the graph
    TGraphErrors *graph = new TGraphErrors(nPoints, V_multimeter, V_oscilloscope, V_multimeter_err, V_oscilloscope_err);
    graph->SetTitle("Voltage Calibration;V_{multimetro} (V);V_{oscilloscopio} (V)");
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(1.0);

    // Define and perform a linear fit
    TF1 *fitFunc = new TF1("fitFunc", "[0] + [1]*x", 0, 800);
    fitFunc->SetLineColor(kRed);
    fitFunc->SetLineWidth(2);
    graph->Fit(fitFunc, "Q");

    // Print the fit results
    std::cout << "Intercept (a): " << fitFunc->GetParameter(0) << " ± " << fitFunc->GetParError(0) << std::endl;
    std::cout << "Slope (b): " << fitFunc->GetParameter(1) << " ± " << fitFunc->GetParError(1) << std::endl;

    // Create and display the canvas
    TCanvas *c1 = new TCanvas("c1", "Linear Fit", 800, 600);
    graph->Draw("AP");

    // Enter the ROOT event loop to display the window
    app.Run();

    // Clean up
    delete graph;
    delete fitFunc;
    delete c1;

    return 0;
}
