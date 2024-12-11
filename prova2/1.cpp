#include <iostream>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TAxis.h>

void I_V1()
{
    // Definizione dei punti dati
    const int nPoints = 26;
    double V_oscilloscope[nPoints] = {4, 3.5, 3.25, 3, 2.75, 2.5, 2.25, 2, 1.75, 1.5, 1.25, 1, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.35, 0.3, 0.25, 0.2, 0.15, 0.1, 0.055, 0.05};
    double I_multimeter[nPoints] = {17.17, 17.01, 16.79, 16.71, 16.54, 16.38, 16.2, 16.09, 15.98, 15.72, 15.53, 15.33, 15.22, 15.12, 14.99, 14.85, 14.7, 14.42, 14.08, 13.47, 12.42, 9.82, 5.83, 2.34, 0.72, 0.57};
    double V_oscilloscope_err[nPoints] = {0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005};
    double I_multimeter_err[nPoints] = {0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005};

    // Creazione del grafico
    TGraphErrors *graph = new TGraphErrors(nPoints, V_oscilloscope, I_multimeter, V_oscilloscope_err, I_multimeter_err);
    graph->SetTitle("I vs V;V_{oscilloscopio} (mV);I_{multimetro} (mV)");
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(0.5);

    // Definizione della funzione di fit lineare
    TF1 *fitFunc = new TF1("fitFunc", "[0] + [1]*x", 0.55, 4.5);
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
    graph->GetYaxis()->SetLimits(0, 20);
    graph->SetMinimum(0); // Imposta il minimo dell'asse Y

    // Disegna il grafico
    graph->Draw("AP");

    // Nota: non eliminiamo manualmente gli oggetti
}
