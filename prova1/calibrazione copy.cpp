#include <iostream>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TAxis.h>

void calibrazione()
{
    // Definizione dei punti dati
    const int nPoints = 12;
    double V_multimeter[nPoints] = {47, 77, 96, 147, 197, 245, 300, 390, 490, 590, 690, 790};
    double V_oscilloscope[nPoints] = {50, 70, 100, 150, 200, 250, 300, 400, 500, 600, 700, 800};
    double V_multimeter_err[nPoints] = {2, 3, 4, 5, 7, 8, 10, 13, 16, 18, 20, 20};
    double V_oscilloscope_err[nPoints] = {2, 3, 3, 7, 6, 8, 9, 16, 15, 20, 30, 24};

    // Creazione del grafico
    TGraphErrors *graph = new TGraphErrors(nPoints, V_multimeter, V_oscilloscope, V_multimeter_err, V_oscilloscope_err);
    graph->SetTitle("Calibrazione Tensione;V_{multimetro} (mV);V_{oscilloscopio} (mV)");
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(1.0);

    // Definizione della funzione di fit lineare
    TF1 *fitFunc = new TF1("fitFunc", "[0] + [1]*x", 0, 900);
    fitFunc->SetLineColor(kRed);
    fitFunc->SetLineWidth(2);

    // Fit dei dati
    graph->Fit(fitFunc, "Q");

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
    graph->GetXaxis()->SetLimits(0, 900);
    graph->SetMinimum(0); // Imposta il minimo dell'asse Y

    // Disegna il grafico
    graph->Draw("AP");

    // Nota: non eliminiamo manualmente gli oggetti
}
