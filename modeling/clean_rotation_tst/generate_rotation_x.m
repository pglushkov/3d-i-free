function res = generate_rotation_x(angle, type)

    if (strcmp(type, 'ROW'))
        tr = false;
    elseif (strcmp(type, 'COL'))
        tr = true;
    else
        error('unknown rotation matrix type, specify either ROW or COL type');
    end
    
    S = sin(angle);
    C = cos(angle);
    
    res = [1 0 0; 0 C -S; 0 S C;];

    if(tr)
        res = res';
    end

end