#include <iostream>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TApplication.h>

void plotGraph()
{
    // Define the data points
    const int nPoints = 12;                                                                            // Number of data points
    double V_multimeter[nPoints] = {47, 77, 96, 147, 197, 245, 296, 390, 486, 586, 687, 786};          // X-axis values
    double V_oscilloscope[nPoints] = {50, 70, 100, 150, 200, 250, 300, 400, 500, 600, 700, 800};       // Y-axis values
    double V_multimeter_err[nPoints] = {0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1};   // Errors in X-axis
    double V_oscilloscope_err[nPoints] = {0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2}; // Errors in Y-axis

    // Create a canvas
    TCanvas *c1 = new TCanvas("c1", "Graph with Errors", 800, 600);

    // Create the graph with error bars
    TGraphErrors *graph = new TGraphErrors(nPoints, V_multimeter, V_oscilloscope, V_multimeter_err, V_oscilloscope_err);

    // Set axis titles
    graph->GetXaxis()->SetTitle("V_{multimetro} (V)");
    graph->GetYaxis()->SetTitle("V_{oscilloscopio} (V)");

    // Set graph styles
    graph->SetTitle("Voltage Calibration;V_{multimetro} (V);V_{oscilloscopio} (V)");
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(1.0);

    // Define a linear fit function: V_oscilloscope = a + b * V_multimeter
    TF1 *fitFunc = new TF1("fitFunc", "[0] + [1]*x", 0, 800); // [0] = intercept (a), [1] = slope (b)
    fitFunc->SetLineColor(kRed);
    fitFunc->SetLineWidth(2);

    // Perform the fit
    graph->Fit(fitFunc, "Q"); // "Q" suppresses fit output for cleaner display

    // Retrieve and print fit parameters
    double a = fitFunc->GetParameter(0);    // Intercept
    double b = fitFunc->GetParameter(1);    // Slope
    double a_err = fitFunc->GetParError(0); // Error in intercept
    double b_err = fitFunc->GetParError(1); // Error in slope

    std::cout << "Fit Results:" << std::endl;
    std::cout << "Intercept (a): " << a << " ± " << a_err << std::endl;
    std::cout << "Slope (b): " << b << " ± " << b_err << std::endl;

    // Draw the graph on the canvas
    graph->Draw("APL"); // "A" draws axis, "P" draws points, "L" draws line

    // Display the canvas in a ROOT window
    c1->Draw();
}

int main(int argc, char **argv)
{
    TApplication app("app", &argc, argv); // Initialize ROOT application
    plotGraph();                          // Call your function to plot the graph
    app.Run();                            // Run the ROOT application to keep the window open
    return 0;
}
