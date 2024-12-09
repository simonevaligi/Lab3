#include <iostream>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TAxis.h>

void calibrazione()
{
    // Definizione dei punti dati
    const int nPoints = 12;
    double V_multimeter[nPoints] = {47, 77, 96, 147, 197, 245, 296, 390, 486, 586, 687, 786};
    double V_oscilloscope[nPoints] = {50, 70, 100, 150, 200, 250, 300, 400, 500, 600, 700, 800};
    double V_multimeter_err[nPoints] = {2.41, 3.31, 3.88, 5.41, 6.91, 8.35, 9.88, 12.7, 15.6, 18.6, 21.6, 24.6};
    double V_oscilloscope_err[nPoints] = {1.80, 2.90, 3.00, 6.73, 6.00, 7.50, 9.00, 15.6, 15.0, 18.0, 29.0, 24.0};

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
