#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include "pdfgen.h" 

struct Producte {
    std::string nom;
    std::string categoria;
    double preu;
};

int main() {
    std::vector<Producte> productes = {
        {"Llibre A", "Educació", 12.5},
        {"Llibre B", "Literatura", 9.0},
        {"Llibre C", "Educació", 15.0},
        {"Llibre D", "Ficció", 8.5}
    };

    // Crear un documento PDF
    struct pdf_doc *pdf = pdf_create(PDF_A4_WIDTH, PDF_A4_HEIGHT, NULL);
    if (!pdf) {
        std::cerr << "Error al crear el document PDF.\n";
        return 1;
    }

    // Nueva pagina PDF
    struct pdf_object *page = pdf_append_page(pdf);
    if (!page) {
        std::cerr << "Error al crear la pàgina del PDF.\n";
        pdf_destroy(pdf);
        return 1;
    }

    // Margenes
    int x = 50; 
    int y = 750; 
    int font_size = 12;


    pdf_add_text(pdf, page, "Informe de productes", font_size + 4, x, y, PDF_BLACK);
    y -= 20; 
    std::time_t ara = std::time(nullptr);
    std::string data = "Data: " + std::string(std::asctime(std::localtime(&ara)));
    pdf_add_text(pdf, page, data.c_str(), font_size, x, y, PDF_BLACK);
    y -= 20;
    pdf_add_text(pdf, page, "--------------------------", font_size, x, y, PDF_BLACK);
    y -= 20;

    pdf_add_text(pdf, page, "Nom", font_size, x, y, PDF_BLACK);
    pdf_add_text(pdf, page, "Categoria", font_size, x + 70, y, PDF_BLACK);
    pdf_add_text(pdf, page, "Preu", font_size, x + 150, y, PDF_BLACK);
    y -= 15;
    pdf_add_text(pdf, page, "-----------------------------------------", font_size, x, y, PDF_BLACK);
    y -= 15;

    // Recorrer los productos
    for (const auto& p : productes) {
        if (p.preu > 10) { 
            pdf_add_text(pdf, page, p.nom.c_str(), font_size, x, y, PDF_BLACK);
            pdf_add_text(pdf, page, p.categoria.c_str(), font_size, x + 70, y, PDF_BLACK);
            std::string precio = std::to_string(p.preu) + " €";
            pdf_add_text(pdf, page, precio.c_str(), font_size, x + 150, y, PDF_BLACK);
            y -= 15; 

   
            if (y < 50) {
                page = pdf_append_page(pdf);
                y = 750; 
            }
        }
    }

    // PIE DE PAGINA
    pdf_add_text(pdf, page, "-----------------------------------------", font_size, x, y, PDF_BLACK);
    y -= 20;
    pdf_add_text(pdf, page, "Informe generat automàticament.", font_size, x, y, PDF_BLACK);

    // guardar el pdf
    if (pdf_save(pdf, "informe.pdf") != 0) {
        std::cerr << "Error al guardar el document PDF.\n";
        pdf_destroy(pdf);
        return 1;
    }


    pdf_destroy(pdf);

    std::cout << "Informe generat correctament: informe.pdf\n";
    return 0;
}
