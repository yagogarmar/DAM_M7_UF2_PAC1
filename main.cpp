#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include "pdfgen.h" // Asegúrate de incluir el archivo correcto

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

    // Agregar una página al PDF
    struct pdf_object *page = pdf_append_page(pdf);
    if (!page) {
        std::cerr << "Error al crear la pàgina del PDF.\n";
        pdf_destroy(pdf);
        return 1;
    }

    // Configurar posición inicial y tamaño de texto
    int x = 50;  // Margen izquierdo
    int y = 750; // Posición inicial en la parte superior
    int font_size = 12;

    // Escribir la cabecera
    pdf_add_text(pdf, page, "Informe de productes", font_size + 4, x, y, PDF_BLACK);
    y -= 20; // Mover hacia abajo
    std::time_t ara = std::time(nullptr);
    std::string data = "Data: " + std::string(std::asctime(std::localtime(&ara)));
    pdf_add_text(pdf, page, data.c_str(), font_size, x, y, PDF_BLACK);
    y -= 20;
    pdf_add_text(pdf, page, "--------------------------", font_size, x, y, PDF_BLACK);
    y -= 20;

    // Escribir los encabezados de la tabla
    pdf_add_text(pdf, page, "Nom", font_size, x, y, PDF_BLACK);
    pdf_add_text(pdf, page, "Categoria", font_size, x + 70, y, PDF_BLACK);
    pdf_add_text(pdf, page, "Preu", font_size, x + 150, y, PDF_BLACK);
    y -= 15;
    pdf_add_text(pdf, page, "-----------------------------------------", font_size, x, y, PDF_BLACK);
    y -= 15;

    // Escribir los detalles de los productos
    for (const auto& p : productes) {
        if (p.preu > 10) { // Filtrar productos con precio mayor a 10
            pdf_add_text(pdf, page, p.nom.c_str(), font_size, x, y, PDF_BLACK);
            pdf_add_text(pdf, page, p.categoria.c_str(), font_size, x + 70, y, PDF_BLACK);
            std::string precio = std::to_string(p.preu) + " €";
            pdf_add_text(pdf, page, precio.c_str(), font_size, x + 150, y, PDF_BLACK);
            y -= 15; // Mover hacia abajo para la siguiente línea

            // Si se sale de la página, puedes agregar una nueva página
            if (y < 50) {
                page = pdf_append_page(pdf);
                y = 750; // Restablecer posición en la nueva página
            }
        }
    }

    // Escribir el pie de página
    pdf_add_text(pdf, page, "-----------------------------------------", font_size, x, y, PDF_BLACK);
    y -= 20;
    pdf_add_text(pdf, page, "Informe generat automàticament.", font_size, x, y, PDF_BLACK);

    // Guardar el PDF a un archivo
    if (pdf_save(pdf, "informe.pdf") != 0) {
        std::cerr << "Error al guardar el document PDF.\n";
        pdf_destroy(pdf);
        return 1;
    }

    // Limpiar y liberar memoria
    pdf_destroy(pdf);

    std::cout << "Informe generat correctament: informe.pdf\n";
    return 0;
}
