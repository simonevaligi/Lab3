#include <iostream>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TAxis.h>

void I_V2()
{
    // Definizione dei punti dati
    const int nPoints = 25;
    double V_oscilloscope[nPoints] = {4, 3.5, 3.25, 3, 2.75, 2.5, 2.25, 2, 1.75, 1.5, 1.25, 1, 0.9, 0.8, 0.7, 0.5, 0.4, 0.35, 0.3, 0.25, 0.2, 0.15, 0.1, 0.055, 0.05};
    double I_multimeter[nPoints] = {32.3, 32.04, 31.12, 31.11, 30.76, 30.45, 29.97, 29.52, 29.1, 28.54, 27.9, 27.42, 27.08, 26.68, 26.38, 25.19, 24.26, 23.71, 22.54, 20.45, 16.49, 9.95, 4.74, 1.38, 1.11};
    double V_oscilloscope_err[nPoints] = {0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005};
    double I_multimeter_err[nPoints] = {0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005};

    // Creazione del grafico
    TGraphErrors *graph = new TGraphErrors(nPoints, V_oscilloscope, I_multimeter, V_oscilloscope_err, I_multimeter_err);
    graph->SetTitle("I vs V;V_{oscilloscopio} (mV);I_{multimetro} (mV)");
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(0.5);

    // Definizione della funzione di fit lineare
    TF1 *fitFunc = new TF1("fitFunc", "[0] + [1]*x", 0.6, 4.5);
    fitFunc->SetLineColor(kRed);
    fitFunc->SetLineWidth(2);

    // Fit dei dati
    graph->Fit(fitFunc, "RQ"); // Usa il flag "RQ" per forzare l'intervallo

    // Parametri di fit
    double a = fitFunc->GetParameter(0);
    double b = fitFunc->GetParameter(1);
    double a_err = fitFunc->GetParError(0);
    double b_err = fitFunc->GetParError(1);

    std::cout << "Risultati del fit:" << std::endl;
    std::cout << "Intercetta (a): " << a << " ± " << a_err << std::endl;
    std::cout << "Pendenza (b): " << b << " ± " << b_err << std::endl;

    // Creazione del canvas
    TCanvas *c1 = new TCanvas("c1", "Linear Fit", 600, 600);
    c1->SetGrid();

    // Imposta il limite minimo degli assi X e Y a zero per far coincidere lo zero
    graph->GetXaxis()->SetLimits(0, 4.5);
    graph->GetYaxis()->SetLimits(0, 35);
    graph->SetMinimum(0); // Imposta il minimo dell'asse Y

    // Disegna il grafico
    graph->Draw("AP");

    // Nota: non eliminiamo manualmente gli oggetti
}
