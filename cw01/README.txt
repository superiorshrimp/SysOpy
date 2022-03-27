wykonane zadania:
    -zadanie 1
    -zadanie 2
    -zadanie 3a
    -zadanie 3b
    (wszystkie)

wyniki testow zebrane sa w cw01:
    raport2.txt - wyniki testow z zadania 2.
    results3a.txt - wyniki testow i ich analiza z zadania 3a
    results3b.txt - wyniki testow i ich analiza z zadania 3b
    results_static, results_shared, results_dynamic (.txt) - czastkowe wyniki testow z zadania 3a
    results_static_opt, results_shared_opt, results_dynamic_opt (.txt) - czastkowe wyniki testow z zadania 3b 

instrukcja poruszania się po zadaniach:
    zadanie 1:
        1. make static - stworzy biblioteke statyczna (.a)
        2. make shared - stworzy biblioteke dzielona (.so)
        3. make clean - wroci do stanu z przed wywołania make'ow
    zadanie 2:
        1. make main - utworzy wykonywalny program
        2. make testing - przeprowadzi wymagane w poleceniu testy (nalezy wczesniej wykonac make main)
        3. make clean - wróci do stanu z przed wywołania make'ow
        *4. sposob wprowadzania argumentow do dodatkowych testow do programu main opisany jest dokladnie w pliku zad2/main.c
    zadanie 3a:
        1. make static - utworzy wykonywalny program uzywajacy statycznej biblioteki
        2. make tests_static - wykona testy (nalezy nalezy wczesniej wykonac make static)
        3. make shared - utworzy wykonywalny program uzywajacy dzielonej biblioteki
        4. make tests_shared - wykona testy (nalezy nalezy wczesniej wykonac make shared)
        5. make dynamic - utworzy wykonywalny program uzywajacy dynamicznie ładowanej biblioteki
        6. make tests_dynamic - wykona testy (nalezy wczesniej wykonac make dynamic)
        7. make clean - wróci do stanu z przed wywołania make'ow
    zadanie 3b:
        1. make static - wykona testy dla podanych na upelu optymalizacji dla biblioteki statycznej (O0, O1...)
        2. make shared - wykona testy dla podanych na upelu optymalizacji dla biblioteki dzielonej (O0, O1...)
        3. make dynamic - wykona testy dla podanych na upelu optymalizacji dla biblioteki dynamicznej (O0, O1...)
        4. make clean - wróci do stanu z przed wywołania make'ow
