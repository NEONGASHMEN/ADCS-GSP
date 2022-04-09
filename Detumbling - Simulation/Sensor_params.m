%%Demo snsors for mag filed and ang velocity measurement

Magsnsr_bias_scale = 4e-7; %in T
Magsnsr_bias = Magsnsr_bias_scale*(2*rand() - 1);

Magsnsr_noise_scale = 1e-5;
Magsnsr_noise = Magsnsr_noise_scale*(2*rand() - 1);

Angsnsr_bias_scale = 0.01; %in rad/s
Angsnsr_bias = Angsnsr_bias_scale*(2*rand() - 1);

Angsnsr_noise_scale = 0.001;
Angsnsr_noise = Angsnsr_noise_scale*(2*rand() - 1);
