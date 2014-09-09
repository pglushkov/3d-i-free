function res = generate_rotation_y(angle, type)

    if (strcmp(type, 'ROW'))
        tr = false;
    elseif (strcmp(type, 'COL'))
        tr = true;
    else
        error('unknown rotation matrix type, specify either ROW or COL type');
    end
    
    S = sin(angle);
    C = cos(angle);
    
    res = [C 0 S; 0 1 0; -S 0 C;];

    if(tr)
        res = res';
    end

end