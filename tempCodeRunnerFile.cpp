
            registers[write_reg] = convert_binary_to_num(stoi(var1)); // Write memory data to register
        } else {
            cout<<"Write Data: "<<alures<<endl; // Write ALU result
            registers[write_reg] = alures; // Write ALU result to register
        }
